#!/usr/bin/env python3
"""Data from the ROM: write one .inc.c of C literals per row of config/AWRE/data_inc.txt.

    data_inc.py write [--out extracted/AWRE/data]   extract/ -> <out>/<module>/<symbol>.inc.c
    data_inc.py outputs                             print the paths `write` makes, one per line

Some data sits where the code is: a table in an ITCM .text range, the module
parameters after the ARM9 crt0. Its values are game data and never enter git
(tenet 9). Committed C defines the symbol and includes the values inside the
initializer:

    u32 gUnit3dTextureBlocks[][27] = {
    #include "data/itcm/gUnit3dTextureBlocks.inc.c"
    };

`make setup` writes the files under extracted/ (gitignored), and the build
writes them again when an input changes (docs/workflow.md, "Data from the
ROM"). The pattern of including generated .inc.c files in array initializers
comes from zeldaret/oot (docs/third-party/zeldaret-oot.md); no code is taken.

A row of data_inc.txt: module, symbol, address, size in bytes, element type.

    itcm gUnit3dTextureBlocks 0x01ffb9bc 0x288 u32[27]

- module: main, itcm, dtcm, ovNNN or arm7.
- type: u8, s8, u16, s16, u32, s32, u64 (written with ULL; mwccarm aligns it
  to a word, as in PowerLedBlinkScript) or a pointer type (`void*`, `char*`),
  with an optional `[N]` that writes the values as rows of N in braces.
  For a struct or an array of structs, the field types in braces, no
  spaces, with `[N]` for an array field: `{u8,u8,u8[2],u32,u32,u16*,u8,u8[3]}`
  writes one line of values per struct, with no braces in it, so the
  same file initializes one struct or an array of them (C fills the
  members in order). `pad[N]` stands for N bytes of padding, which must be
  zero and get no value. Fields must be aligned to their size. A number
  field may name its values: `u8:EVENT_OP` writes a value as the
  `#define EVENT_OP_...` under include/ that has it, when exactly one
  does, and as a number otherwise; the C file includes that header.
  `u8:4` is a bit field of 4 bits: as mwcc does, bit fields of one type
  fill a unit of that type from the low end, and one that does not fit
  starts the next unit. `u32:0` ends the unit and takes no value, as in C;
  it stands where one struct of bit fields ends and the next begins.
  `u8[28+1=100]` is an array field that the C struct has grown: 28 values
  come from the ROM and one more of value 100 follows, for a table indexed
  by an id that a mod adds. `u8[28+1=@11]` repeats element 11 instead.

A word that the module's relocs.txt says points somewhere is written as the
symbol at its target (`to` plus `add`), `(u32)&Name` or `(void*)&Name`, so a
build with moved code keeps it right. A target where no symbol starts but
that is the retail value of one of configure.py's ARM9_LAYOUT_SYMBOLS is
written as that linker symbol (crt0's autoload list in BuildInfo, issue
0086); otherwise as the symbol below it plus an offset. The C file that includes the values declares those
symbols. A pointer element that is neither 0 nor relocated stops the tool.
"""

import argparse
import re
import sys
from pathlib import Path

import yaml

import arm7  # regions(): where each ARM7 part sits in arm7.bin

ROOT = Path(__file__).resolve().parent.parent
LIST = Path("config/AWRE/data_inc.txt")
OUT = Path("extracted/AWRE/data")
EXTRACT = Path("extract")
CONFIG9 = Path("config/AWRE/arm9")
CONFIG7 = Path("config/AWRE/arm7")

SIZES = {"u8": 1, "s8": 1, "u16": 2, "s16": 2, "u32": 4, "s32": 4, "u64": 8}
ALIGN_MAX = 4  # mwccarm aligns a u64 like a word
PER_LINE = {1: 16, 2: 8, 4: 8, 8: 4}
ROW = re.compile(r"(\S+)\s+(\w+)\s+(0x[0-9a-fA-F]+)\s+(0x[0-9a-fA-F]+|\d+)\s+(\w+\s*\**|\{[^\s{}]+\})(?:\[(\d+)\])?$")
FIELD = re.compile(r"(\w+\**)(?::([A-Z][A-Z0-9_]*)|:(\d+))?(?:\[(\d+)(?:\+(\d+)=(@?\d+))?\])?$")
PAD = "pad"  # a struct field of padding bytes


class Row:
    def __init__(self, line: str, where: str) -> None:
        m = ROW.match(line)
        if not m:
            sys.exit(f"error: {where}: expected `module symbol address size type[N]`")
        self.module, self.symbol = m.group(1), m.group(2)
        self.addr, self.size = int(m.group(3), 16), int(m.group(4), 0)
        self.type = m.group(5).replace(" ", "")
        self.columns = int(m.group(6)) if m.group(6) else 0
        self.pointer = self.type.endswith("*")
        self.fields: list[tuple[str, int]] = []  # (type, count; 0 for a single value)
        self.names: list[str | None] = []  # per field: the #define prefix its values are named by
        self.layout: list[tuple[int, int, int]] = []  # per field: (offset, first bit, bits; 0 if not a bit field)
        self.grow: list[tuple[int, str] | None] = []  # per array field: (values added, `V` or `@I`)
        if self.type.startswith("{"):
            self._struct(where)
            return
        if not self.pointer and self.type not in SIZES:
            sys.exit(f"error: {where}: unknown element type {self.type!r}")
        self.width = 4 if self.pointer else SIZES[self.type]
        if self.size % self.width or self.addr % min(self.width, ALIGN_MAX):
            sys.exit(f"error: {where}: address and size must be multiples of {self.width}")
        if self.columns and (self.size // self.width) % self.columns:
            sys.exit(f"error: {where}: {self.size // self.width} elements do not make rows of {self.columns}")
        self.where = where

    def _struct(self, where: str) -> None:
        offset = 0
        unit = None  # the open bit field unit: [type, offset, next free bit]
        for text in self.type[1:-1].split(","):
            m = FIELD.match(text)
            if not m or not (m.group(1).endswith("*") or m.group(1) in SIZES or m.group(1) == PAD):
                sys.exit(f"error: {where}: bad struct field {text!r}")
            width = 4 if m.group(1).endswith("*") else 1 if m.group(1) == PAD else SIZES[m.group(1)]
            if m.group(2) and m.group(1) not in SIZES:
                sys.exit(f"error: {where}: field {text!r}: only a number field takes constant names")
            count = int(m.group(4)) if m.group(4) else 0
            bits = int(m.group(3)) if m.group(3) else 0
            grow = (int(m.group(5)), m.group(6)) if m.group(5) else None
            if grow and (m.group(1) not in SIZES or bits or m.group(2)
                         or grow[1].startswith("@") and int(grow[1][1:]) >= count):
                sys.exit(f"error: {where}: bad grown array field {text!r}")
            if m.group(3) == "0":
                if m.group(1) not in SIZES or count:
                    sys.exit(f"error: {where}: bad bit field {text!r}")
                unit = None
                continue
            if bits:
                # mwcc fills a bit field unit from the low end and opens a
                # new one when the next field of the same type does not fit
                if m.group(1) not in SIZES or count or bits > 8 * width:
                    sys.exit(f"error: {where}: bad bit field {text!r}")
                if not unit or unit[0] != m.group(1) or unit[2] + bits > 8 * width:
                    if offset % width:
                        sys.exit(f"error: {where}: field {text!r} at offset {offset:#x} is not aligned")
                    unit = [m.group(1), offset, 0]
                    offset += width
                self.layout.append((unit[1], unit[2], bits))
                unit[2] += bits
            else:
                unit = None
                if offset % min(width, ALIGN_MAX):
                    sys.exit(f"error: {where}: field {text!r} at offset {offset:#x} is not aligned")
                self.layout.append((offset, 0, 0))
                offset += width * max(count, 1)
            self.fields.append((m.group(1), count))
            self.names.append(m.group(2))
            self.grow.append(grow)
        self.width = offset
        if self.columns or self.size % self.width or self.addr % 4:
            sys.exit(f"error: {where}: {self.size:#x} bytes are not whole structs of {self.width:#x}")
        self.where = where

    @property
    def rel(self) -> str:
        return f"{self.module}/{self.symbol}.inc.c"


def read_list(path: Path = LIST) -> list[Row]:
    rows = []
    for n, line in enumerate((ROOT / path).read_text().splitlines(), 1):
        line = line.split("#", 1)[0].strip()
        if line:
            rows.append(Row(line, f"{path}:{n}"))
    return rows


def config_dir(module: str) -> Path:
    if module == "arm7":
        return CONFIG7
    if module == "main":
        return CONFIG9
    if module in ("itcm", "dtcm"):
        return CONFIG9 / module
    return CONFIG9 / "overlays" / module


def module_key(name: str) -> str:
    """The module name used here for a relocs.txt `module:` field."""
    m = re.match(r"overlay\((\d+)\)$", name)
    return f"ov{int(m.group(1)):03d}" if m else name


DEFINE = re.compile(r"^#define\s+([A-Z][A-Z0-9_]*)\s+(?:\\\n\s*)?(0x[0-9a-fA-F]+|\d+)\b", re.M)
INCLUDE = Path("include")
_constants: dict[str, tuple[set[str], dict[int, str]]] = {}


def constants(prefix: str) -> tuple[set[str], dict[int, str]]:
    """The headers under include/ that #define names PREFIX_*, and value ->
    name for each value exactly one of those names has."""
    if prefix not in _constants:
        headers, names = set(), {}
        for h in sorted((ROOT / INCLUDE).rglob("*.h")):
            for m in DEFINE.finditer(h.read_text(errors="replace")):
                if m.group(1).startswith(prefix + "_"):
                    headers.add((INCLUDE / h.relative_to(ROOT / INCLUDE)).as_posix())
                    names.setdefault(int(m.group(2), 0), []).append(m.group(1))
        _constants[prefix] = (headers, {v: n[0] for v, n in names.items() if len(n) == 1})
    return _constants[prefix]


def config_inputs(rows: list[Row]) -> list[str]:
    """The files `write` reads from config/: relocs.txt of each row's module,
    and symbols.txt of every module a relocation may point into; and the
    headers that name a field's values."""
    files = {(config_dir(r.module) / "relocs.txt").as_posix() for r in rows}
    for prefix in {n for r in rows for n in r.names if n}:
        files |= constants(prefix)[0]
    for cpu in {CONFIG7 if r.module == "arm7" else CONFIG9 for r in rows}:
        files |= {p.relative_to(ROOT).as_posix() for p in (ROOT / cpu).rglob("symbols.txt")}
    return sorted(files)


class Rom:
    """Module bytes from extract/, with symbols and relocations from config/."""

    def __init__(self) -> None:
        self.images: dict[str, tuple[int, bytes]] = {}
        self.symbols: dict[str, dict[int, str]] = {}
        self.relocs: dict[str, dict[int, tuple[str, int, str]]] = {}
        self.thumb: dict[str, set[int]] = {}  # addresses of Thumb functions

    def image(self, module: str) -> tuple[int, bytes]:
        if module not in self.images:
            self.images[module] = self._load(module)
        return self.images[module]

    def _load(self, module: str) -> tuple[int, bytes]:
        if module == "arm7":
            return 0, (ROOT / EXTRACT / "arm7/arm7.bin").read_bytes()  # read() maps addresses by part
        if module in ("main", "itcm", "dtcm"):
            name = "arm9" if module == "main" else module
            info = yaml.safe_load((ROOT / EXTRACT / f"arm9/{name}.yaml").read_text())
            return info["base_address"], (ROOT / EXTRACT / f"arm9/{name}.bin").read_bytes()
        m = re.fullmatch(r"ov(\d{3})", module)
        if not m:
            sys.exit(f"error: unknown module {module!r}")
        info = yaml.safe_load((ROOT / EXTRACT / "arm9_overlays/overlays.yaml").read_text())
        ov = next((o for o in info["overlays"] if o["id"] == int(m.group(1))), None)
        if ov is None:
            sys.exit(f"error: no overlay {module} in extract/")
        return ov["base_address"], (ROOT / EXTRACT / "arm9_overlays" / ov["file_name"]).read_bytes()

    def read(self, module: str, addr: int, size: int) -> bytes:
        base, data = self.image(module)
        if module == "arm7":
            region = arm7.region_of(arm7.regions(data), addr)
            if region is None or addr + size > region.base + region.size:
                sys.exit(f"error: arm7 {addr:#x}+{size:#x} is not in an ARM7 part's code or data")
            start = region.offset + addr - region.base
        else:
            start = addr - base
            if start < 0 or start + size > len(data):
                sys.exit(f"error: {module} {addr:#x}+{size:#x} is outside the module")
        return data[start:start + size]

    def symbol_table(self, module: str) -> dict[int, str]:
        if module not in self.symbols:
            table: dict[int, str] = {}
            thumb = self.thumb.setdefault(module, set())
            path = ROOT / config_dir(module) / "symbols.txt"
            for line in path.read_text().splitlines():
                m = re.match(r"(\S+) kind:(\S+) addr:(0x[0-9a-f]+)", line)
                if m:
                    table.setdefault(int(m.group(3), 16), m.group(1))
                    if m.group(2).startswith("function(thumb"):
                        thumb.add(int(m.group(3), 16))
            self.symbols[module] = table
        return self.symbols[module]

    def reloc_table(self, module: str) -> dict[int, tuple[str, int, str]]:
        """from -> (kind, target address (to plus add), target module)."""
        if module not in self.relocs:
            table = {}
            path = ROOT / config_dir(module) / "relocs.txt"
            for line in path.read_text().splitlines():
                m = re.match(r"from:(0x\w+) kind:(\S+) to:(0x\w+)(?: add:(-?0x\w+))? module:(\S+)", line)
                if m:
                    to = int(m.group(3), 16) + int(m.group(4) or "0", 16)
                    table[int(m.group(1), 16)] = (m.group(2), to, m.group(5))
            self.relocs[module] = table
        return self.relocs[module]

    def target(self, module: str, to: int, where: str) -> tuple[str, int]:
        """(symbol, offset) for a relocation target: the symbol at `to`, a
        layout symbol whose retail value is `to`, or the nearest symbol below
        it. A Thumb function's address has bit 0 set, which the linker adds
        to `&function` itself."""
        table = self.symbol_table(module)
        if to in table:
            return table[to], 0
        if to & 1 and to - 1 in self.thumb.get(module, ()):
            return table[to - 1], 0
        layout = layout_symbols(module)
        if to in layout:
            return layout[to], 0
        below = [a for a in table if a <= to]
        if not below:
            sys.exit(f"error: {where}: no symbol at or below {to:#x} in {module}")
        base = max(below)
        return table[base], to - base


def layout_symbols(module: str) -> dict[int, str]:
    """Retail value -> name of the symbols the linker script computes that
    a module's data may point at: configure.py's ARM9_LAYOUT_SYMBOLS for
    ARM9, none for ARM7."""
    if module == "arm7":
        return {}
    import configure  # here, not at the top: configure imports this module

    return {retail: name for name, (_, retail) in configure.ARM9_LAYOUT_SYMBOLS.items()}


def literal(row: Row, rom: Rom, addr: int, raw: bytes, cast: str = "") -> str:
    cast = cast or row.type
    reloc = rom.reloc_table(row.module).get(addr) if len(raw) == 4 else None
    value = int.from_bytes(raw, "little", signed=cast.startswith("s"))
    if reloc:
        kind, to, target = reloc
        where = f"{row.symbol} at {addr:#x}"
        if kind != "load":
            sys.exit(f"error: {where}: relocation kind {kind} is not a data word")
        if target.startswith("overlays("):
            sys.exit(f"error: {where}: relocation into several overlays ({target}); give it one module")
        name, offset = rom.target(module_key(target), to, where)
        if offset:
            return f"({cast})((char*)&{name} + {offset:#x})"
        return f"({cast})&{name}"
    if cast.endswith("*"):
        if value:
            sys.exit(f"error: {row.symbol} at {addr:#x}: pointer {value:#x} has no relocation in relocs.txt")
        return "0"
    if cast.startswith("s"):
        return str(value)
    return f"{value:#0{2 + 2 * len(raw)}x}" + ("ULL" if len(raw) == 8 else "")


def render_struct(row: Row, rom: Rom, addr: int, data: bytes) -> str:
    """One struct as `a, b, c, d, ...,`: no braces around it or around its
    array members, so C fills the members in order whether the object is
    one struct or an array. A brace at the start of a line would take the
    whole element when the struct's first member is an array."""
    parts = []
    for (cast, count), prefix, (offset, bit, bits), grow in zip(row.fields, row.names, row.layout, row.grow):
        if bits:
            value = int.from_bytes(data[offset:offset + SIZES[cast]], "little") >> bit & ((1 << bits) - 1)
            if cast.startswith("s") and value >> (bits - 1):
                value -= 1 << bits
            parts.append(str(value) if cast.startswith("s") else f"{value:#x}")
            continue
        if cast == PAD:
            if any(data[offset:offset + count]):
                sys.exit(f"error: {row.symbol} at {addr + offset:#x}: padding bytes are not zero")
            continue
        width = 4 if cast.endswith("*") else SIZES[cast]
        values = []
        for i in range(max(count, 1)):
            at = offset + i * width
            raw = data[at:at + width]
            name = constants(prefix)[1].get(int.from_bytes(raw, "little", signed=cast.startswith("s"))) if prefix else None
            values.append(name or literal(row, rom, addr + at, raw, cast))
        if grow:
            added, fill = grow
            if fill.startswith("@"):
                value = values[int(fill[1:])]
            else:
                value = fill if cast.startswith("s") else f"{int(fill):#0{2 + 2 * width}x}"
            values += [value] * added
        parts += values
    return ", ".join(parts) + ","


def render(row: Row, rom: Rom) -> str:
    data = rom.read(row.module, row.addr, row.size)
    if row.fields:
        lines = [f"/* {row.symbol}: {row.module} {row.addr:#010x}, {row.size:#x} bytes of structs."
                 " Written by tools/data_inc.py from the ROM; do not edit. */"]
        for i in range(0, row.size, row.width):
            lines.append(render_struct(row, rom, row.addr + i, data[i:i + row.width]))
        return "\n".join(lines) + "\n"
    values = [literal(row, rom, row.addr + i, data[i:i + row.width]) for i in range(0, row.size, row.width)]
    lines = [f"/* {row.symbol}: {row.module} {row.addr:#010x}, {row.size:#x} bytes of {row.type}."
             " Written by tools/data_inc.py from the ROM; do not edit. */"]
    if row.columns:
        for i in range(0, len(values), row.columns):
            chunk = values[i:i + row.columns]
            per = PER_LINE[row.width]
            parts = [", ".join(chunk[j:j + per]) for j in range(0, len(chunk), per)]
            lines.append("{" + (",\n ".join(parts)) + "},")
    else:
        per = PER_LINE[row.width]
        lines += [", ".join(values[i:i + per]) + "," for i in range(0, len(values), per)]
    return "\n".join(lines) + "\n"


def check_symbol(row: Row, rom: Rom) -> None:
    """The symbol, if symbols.txt has it, must sit at the row's address."""
    table = rom.symbol_table(row.module)
    addrs = [a for a, n in table.items() if n == row.symbol]
    if addrs and row.addr not in addrs:
        sys.exit(f"error: {row.where}: symbols.txt puts {row.symbol} at {addrs[0]:#x}, not {row.addr:#x}")


def cmd_write(args) -> int:
    rom = Rom()
    out = ROOT / args.out
    rows = read_list()
    for row in rows:
        check_symbol(row, rom)
        path = out / row.rel
        text = render(row, rom)
        # Only on change, so the C files that include it are not rebuilt.
        if not path.exists() or path.read_text() != text:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(text)
    if not args.quiet:
        print(f"data_inc: {len(rows)} files in {args.out}")
    return 0


def cmd_outputs(args) -> int:
    for row in read_list():
        print((OUT / row.rel).as_posix())
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = parser.add_subparsers(required=True)
    p = sub.add_parser("write", help="write the .inc.c files from extract/")
    p.add_argument("--out", default=str(OUT))
    p.add_argument("--quiet", action="store_true")
    p.set_defaults(func=cmd_write)
    p = sub.add_parser("outputs", help="list the files `write` makes")
    p.set_defaults(func=cmd_outputs)
    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
