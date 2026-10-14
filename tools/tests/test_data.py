#!/usr/bin/env python3
"""Checks tools/data.py on made-up bytes (docs/data-tool.md).

    python3 tools/tests/test_data.py

No ROM needed. Covers the string test (string_size), const for .rodata
(const_base), the array counts a declaration gives (fit_dims), a struct's
data_inc type with padding (struct_row), the type chosen from the bytes
when nothing declares the object (plain_shape: string, pointers, words,
halfwords, bytes), the written-out initializer of a pointer table, and
the edits to symbols.txt and data_inc.txt. With the compiler from `make
setup`, also compiles definitions written by `definition` and
`block_lines` and checks that they come out in address order in .data,
with an all-zero object kept there by `#pragma explicit_zero_data`, and in
.rodata when const. With build/fnindex.json, build/dsindex.json and
orig/AWRE.nds, runs `data.py plan --show` under PYTHONHASHSEED 1 and 2 on
the small modules and compares the output.
"""

import os
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import bss  # noqa: E402
import data  # noqa: E402
import data_inc  # noqa: E402
import pragmas  # noqa: E402

ROOT = data.ROOT
SMALL = ["dtcm", "ov003", "ov004", "ov005", "ov006", "arm7"]
failures = []


def check(what: str, ok: bool, detail="") -> None:
    print(f"{what}: {'ok' if ok else 'FAIL'}{f' ({detail})' if detail and not ok else ''}")
    if not ok:
        failures.append(what)


def refused(fn) -> bool:
    try:
        fn()
    except data.Refused:
        return True
    return False


class FakeRom(data_inc.Rom):
    """One made-up module, `itcm` at 0x1000."""

    def __init__(self, raw: bytes, symbols: dict[int, str], relocs: dict[int, tuple[str, int, str]]) -> None:
        super().__init__()
        self.images["itcm"] = (0x1000, raw)
        self.symbols["itcm"] = symbols
        self.relocs["itcm"] = relocs


class FakeModule:
    """What the shape and definition code reads from a DataModule."""

    def __init__(self, rom: FakeRom) -> None:
        self.name = "itcm"
        self.rom = rom
        self.index = None

    def read(self, addr: int, size: int) -> bytes:
        return self.rom.read(self.name, addr, size)


def words(*values: int) -> bytes:
    return b"".join(v.to_bytes(4, "little") for v in values)


def test_strings() -> None:
    check("string: text, zero, pad", data.string_size(b"abc\0") == 4)
    check("string: 23 bytes and a pad byte", data.string_size(b"x" * 22 + b"\0\0") == 23)
    check("string: too short", data.string_size(b"ab\0\0") is None)
    check("string: not printable", data.string_size(b"ab\x01d\0\0\0\0") is None)
    check("string: more than a pad after it", data.string_size(b"abc\0\0\0\0\0") is None)
    check("string: bytes after the zero", data.string_size(b"abc\0x\0\0\0") is None)


def test_const() -> None:
    check("const: .rodata", data.const_base("u16", ".rodata") == "const u16")
    check("const: .data drops it", data.const_base("const char", ".data") == "char")
    check("const: pointer table in .rodata", data.const_base("u16*", ".rodata") == "u16* const")
    check("const: pointer table in .data", data.const_base("void* const", ".data") == "void*")
    shape = data.Shape("const u16", ["4"])
    check("const: forward declaration in .data", shape.forward("gX", ".data") == "extern u16 gX[];")
    check("const: forward declaration in .rodata", shape.forward("gX", ".rodata") == "extern const u16 gX[];")


def test_dims() -> None:
    v = bss.Var(0x1000, 0x18, 0x18, "gX")
    check("dims: count filled in", data.fit_dims(["", "3"], 2, v) == ["4", "3"])
    check("dims: a macro count stays", data.fit_dims(["COUNT"], 4, v) == ["COUNT"])
    check("dims: wrong count", refused(lambda: data.fit_dims(["5"], 4, v)))
    check("dims: scalar too small", refused(lambda: data.fit_dims([], 4, v)))
    check("dims: not whole elements", refused(lambda: data.fit_dims([""], 0x10, v)))


class StructIndex:
    """A struct with a gap, an array field and a pointer, one with a bit
    field, and one that holds a union."""
    struct_size = {"Layer": 0x10, "Bits": 4, "Ref": 4, "Quote": 8}
    unions = {"Ref"}
    struct_fields = {
        # union { struct { u16 index; u16 group; }; u32 id; }
        "Ref": [{"name": "index", "offset": 0, "size": 2, "type": "u16", "signed": False},
                {"name": "group", "offset": 2, "size": 2, "type": "u16", "signed": False},
                {"name": "id", "offset": 0, "size": 4, "type": "u32", "signed": False}],
        "Quote": [{"name": "ref", "offset": 0, "size": 4, "type": "Ref", "struct": "Ref"},
                  {"name": "text", "offset": 4, "size": 4, "type": "char *", "signed": False}],
        "Layer": [{"name": "mode", "offset": 0, "size": 1, "type": "u8", "signed": False},
                  {"name": "pos", "offset": 2, "size": 4, "type": "s16[2]", "count": 2, "elem_size": 2,
                   "elem_signed": True},
                  {"name": "map", "offset": 8, "size": 4, "type": "u16 *", "signed": False},
                  {"name": "kind", "offset": 12, "size": 1, "type": "u8", "signed": False}],
        "Bits": [{"name": "a", "offset": 0, "size": 4, "type": "u32", "bits": 3}],
    }


def test_struct_row() -> None:
    row, words_at = data.struct_row(StructIndex(), "Layer")
    check("struct: layout with padding", row == "{u8,pad[1],s16[2],pad[2],void*,u8,pad[3]}", row)
    check("struct: where relocations may sit", words_at == [8], words_at)
    data_inc.Row(f"itcm gL 0x1000 0x20 {row}", "test")  # data_inc takes it
    check("struct: bit field refused", refused(lambda: data.struct_row(StructIndex(), "Bits")))
    row, words_at = data.struct_row(StructIndex(), "Quote")
    check("struct: a union as its first member", row == "{u16,u16,void*}", row)
    check("struct: relocations after a union", words_at == [4], words_at)


def test_plain_shapes() -> None:
    rom = FakeRom(b"text\0\0\0\0" + words(0x1100, 0, 0x1104) + words(7) + bytes([1, 0, 2, 0, 3, 0]) + b"\0\0"
                  + bytes([1, 2, 3]) + b"\0",
                  {0x1100: "FuncA", 0x1104: "gData"},
                  {0x1008: ("load", 0x1100, "itcm"), 0x1010: ("load", 0x1104, "itcm")})
    module = FakeModule(rom)
    s = data.plain_shape(module, bss.Var(0x1000, 8, 5, "gText"))
    check("plain: string", (s.base, s.dims, s.row) == ("char", ["5"], "u8"), s)
    s = data.plain_shape(module, bss.Var(0x1008, 12, 12, "gPtrs"))
    check("plain: pointer table written out", (s.base, s.values) == ("void*", ["&FuncA", "0", "&gData"]), s)
    s = data.plain_shape(module, bss.Var(0x1014, 4, 4, "gWord"))
    check("plain: one word", (s.base, s.dims, s.row) == ("u32", [], "u32"), s)
    s = data.plain_shape(module, bss.Var(0x1018, 8, 6, "gHalf"))
    check("plain: halfwords", (s.base, s.dims, s.row) == ("u16", ["3"], "u16"), s)
    s = data.plain_shape(module, bss.Var(0x1020, 4, 3, "gBytes"))
    check("plain: bytes", (s.base, s.dims, s.row) == ("u8", ["3"], "u8"), s)
    rom = FakeRom(words(0x1108), {0x1100: "gData"}, {0x1000: ("load", 0x1108, "itcm")})
    vals = data.pointer_values(FakeModule(rom), bss.Var(0x1000, 4, 4, "gPtr"), "void*")
    check("plain: pointer with an offset", vals == ["(void*)((char*)&gData + 0x8)"], vals)


def test_definition() -> None:
    rom = FakeRom(words(0x1100, 0), {0x1100: "gData"}, {0x1000: ("load", 0x1100, "itcm")})
    module = FakeModule(rom)
    v = bss.Var(0x1000, 8, 8, "gPtrs")
    w = data.definition(module, v, data.Shape("void*", ["2"], values=["&gData", "0"]), ".rodata", True, None,
                        None, True)
    check("definition: const pointer table", w.text == "static void* const gPtrs[2] = {&gData, 0};", w.text)
    w = data.definition(module, v, data.Shape("u32", ["2"], row="u32"), ".data", False, None, None, True)
    check("definition: values from an inc.c file",
          w.text == 'u32 gPtrs[2] = {\n#include "data/itcm/gPtrs.inc.c"\n};', w.text)


def test_edit_symbols() -> None:
    with tempfile.TemporaryDirectory() as d:
        path = Path(d) / "symbols.txt"
        path.write_text("gA kind:data(any) addr:0x01000000\ngB kind:data(any) addr:0x01000010\n"
                        "gB_x kind:data(any) addr:0x01000012\n")
        a = bss.Var(0x01000000, 8, 7, "gA")
        filler = bss.Var(0x01000008, 8, 8, "data_01000008", placeholder=True)
        b = bss.Var(0x01000010, 8, 8, "gB", aliases=[("gB_x", 2)])
        data.edit_symbols(path, [a, filler, b])
        got = path.read_text()
        want = ("gA kind:data(byte[7]) addr:0x01000000\ndata_01000008 kind:data(byte[8]) addr:0x01000008\n"
                "gB kind:data(byte[8]) addr:0x01000010\ngB_x kind:data(byte[0]) addr:0x01000012\n")
        check("symbols.txt: sizes, a placeholder and an alias", got == want, got)


def test_edit_data_inc() -> None:
    with tempfile.TemporaryDirectory() as d:
        root = Path(d)
        (root / data_inc.LIST).parent.mkdir(parents=True)
        (root / data_inc.LIST).write_text("# rows\nitcm      gA  0x01000000  0x4    u32\n"
                                          "itcm      gC  0x01000020  0x4    u32\n")
        rows = [(data_inc.Row("itcm gB 0x01000010 0x8 {u8,pad[3],u32}", "t"), "{u8,pad[3],u32}"),
                (data_inc.Row("ov004 gD 0x02000000 0x4 u32", "t"), "u32")]
        old = data.ROOT
        data.ROOT = root
        try:
            data.edit_data_inc(rows)
        finally:
            data.ROOT = old
        got = [ln.split()[1] for ln in (root / data_inc.LIST).read_text().splitlines() if not ln.startswith("#")]
        check("data_inc.txt: rows in address order, a new module at the end", got == ["gA", "gB", "gC", "gD"], got)


def compile_block(what: str, path: str, section: str, written: list, names: list[str]) -> None:
    probe = data.Probe(None)
    try:
        text = bss.new_file_text(data.block_lines(written, []), set(), False)
        elf = probe.compile(path, text)
        check(f"{what}: compile", elf is not None, data.compile_errors.get((path, text), ""))
        if elf is None:
            return
        g = bss.Group([[w.var for w in written]], str(Path(path).parent), path=path, section=section)
        errors = data.data_errors(elf, g, None)
        check(f"{what}: {names} in address order in {section}", not errors, errors)
    finally:
        probe.close()


def test_probe_retry() -> None:
    """A retry that writes an object from its bytes rewrites its .inc.c."""
    probe = data.Probe(FakeRom(bytes(range(8)), {}, {}))
    try:
        inc = probe.dir / "data" / "itcm" / "gT.inc.c"
        probe.add([(data_inc.Row("itcm gT 0x1000 0x8 u8", "t"), "u8")])
        probe.add([(data_inc.Row("itcm gT 0x1000 0x8 u32", "t"), "u32")])
        text = inc.read_text()
        check("probe: a retry with u32 rewrites the u8 file", "0x03020100" in text, text)
    finally:
        probe.close()


def test_compile() -> None:
    if not pragmas.CC.exists():
        print("test_data: no compiler, run make setup (compile test skipped)")
        return
    sizes = [4, 4, 8, 0x10, 0x10]
    raw = b"".join(bytes([0 if k == 1 else k + 1]) * n for k, n in enumerate(sizes))
    rom = FakeRom(raw, {}, {})
    module = FakeModule(rom)
    written, addr = [], 0x1000
    for k, n in enumerate(sizes):
        v = bss.Var(addr, n, n, f"v_{k}")
        values = ["0" if k == 1 else str(k + 1)] * (n // 4)
        written.append(data.definition(module, v, data.Shape("u32", [str(n // 4)], values=values), ".data",
                                       False, None, None, True))
        addr += n
    check("zero: the block gets the pragma", any(w.zero for w in written))
    compile_block("data", "src/x/data_test.c", ".data", written, [w.var.name for w in written])
    const = [data.Written(w.var, w.shape, w.text.replace("u32 ", "const u32 ", 1), w.zero) for w in written]
    compile_block("rodata", "src/x/rodata_test.c", ".rodata", const, [w.var.name for w in const])
    compile_block("ARM7 data", "src/arm7/x/data_test.c", ".data", written, [w.var.name for w in written])


def test_twice() -> None:
    if not (bss.FNINDEX.exists() and bss.DSINDEX.exists() and pragmas.CC.exists()
            and (ROOT / "orig/AWRE.nds").exists()):
        print("test_data: no build/fnindex.json, build/dsindex.json, compiler or ROM (plan test skipped)")
        return
    for module in SMALL:
        cmd = [sys.executable, "tools/data.py", "plan", module, "--show"]
        first, second = (subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True,
                                        env={**os.environ, "PYTHONHASHSEED": seed}) for seed in ("1", "2"))
        ok = first.returncode == 0 or "past the declared size" in first.stderr or "aliases refused" in first.stderr
        check(f"plan {module} --show runs", ok, first.stderr.strip()[-2000:])
        check(f"plan {module} --show gives the same output under two hash seeds", first.stdout == second.stdout)


def main() -> int:
    test_strings()
    test_const()
    test_dims()
    test_struct_row()
    test_plain_shapes()
    test_definition()
    test_edit_symbols()
    test_edit_data_inc()
    test_probe_retry()
    test_compile()
    test_twice()
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
