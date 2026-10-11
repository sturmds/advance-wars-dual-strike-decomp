#!/usr/bin/env python3
"""Check linker-absolute symbols (configure.py ARM9_ABSOLUTE_SYMBOLS).

    python3 tools/tests/test_absolute.py

- lcf_absolute.py adds each symbol once at the top of SECTIONS.
- fn.py diff (absolute_word) and fn.py check (fnindex.compare) accept a word
  relocated to an absolute symbol where the original holds its value, and
  reject a wrong value or a symbol not in the list.

Uses the compiler from make setup; no ROM or build needed.
"""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import configure  # noqa: E402
import fn  # noqa: E402
import fnindex  # noqa: E402
import lcf_absolute  # noqa: E402

failures = []


def expect(what: str, got, want) -> None:
    print(f"{what}: {'ok' if got == want else 'FAIL'}")
    if got != want:
        failures.append(what)


# lcf_absolute.add
lcf = "MEMORY {\n}\n\nSECTIONS {\n    OVERLAY_0_ID = 0;\n}\n"
once = lcf_absolute.add(lcf, {"B": 0x400, "A": 0})
expect("lcf: added at the top of SECTIONS", once,
       "MEMORY {\n}\n\nSECTIONS {\n    A = 0x00000000;\n    B = 0x00000400;\n    OVERLAY_0_ID = 0;\n}\n")
expect("lcf: second run changes nothing", lcf_absolute.add(once, {"B": 0x400, "A": 0}), once)


# fn.absolute_word, on rows shaped like objdiff-cli's JSON
# A nonzero value, so the check cannot pass on a zero word in our object.
name, value = max(configure.ARM9_ABSOLUTE_SYMBOLS.items(), key=lambda kv: kv[1])
opcode = {"opcode": {"mnemonic": ".word", "opcode": 65534}}


def rows(orig_value: int, sym: str):
    left = {"diff_kind": "DIFF_ARG_MISMATCH",
            "instruction": {"parts": [opcode, {"arg": {"unsigned": str(orig_value)}}]}}
    right = {"diff_kind": "DIFF_ARG_MISMATCH",
             "instruction": {"parts": [opcode, {"arg": {"reloc": True}}],
                             "relocation": {"type": 2, "type_name": "R_ARM_ABS32", "target_symbol": 0}}}
    return {"right": {"symbols": [{"name": sym}]}}, left, right


expect("diff: same value", fn.absolute_word(*rows(value, name)), True)
expect("diff: other value", fn.absolute_word(*rows(value + 4, name)), False)
expect("diff: symbol not in the list", fn.absolute_word(*rows(value, "data_unlisted")), False)


# fnindex.compare on a compiled function that loads the symbol
def compare(sym: str, orig_value: int) -> bool:
    with tempfile.TemporaryDirectory() as d:
        src = Path(d) / "abs.c"
        src.write_text(f"extern char {sym}[];\nunsigned long AbsTest(void) {{ return (unsigned long){sym}; }}\n")
        obj, output = fnindex.compile_object(src)
    if obj is None:
        sys.exit(f"compile error:\n{output}")
    secs, syms, relas = fnindex.read_elf(obj)
    f = next(s for s in syms if s.name == "AbsTest")
    raw = bytearray(secs[f.shndx][1][f.value:f.value + f.size])
    for off, _, _, _ in relas[f.shndx]:
        if f.value <= off < f.value + f.size:
            raw[off - f.value:off - f.value + 4] = orig_value.to_bytes(4, "little")
    entry = {"name": "AbsTest", "module": "main", "mode": "arm", "addr": 0x02000000,
             "raw": raw.hex(), "relocs": []}
    index = fnindex.Index({"AbsTest": [entry]}, {})
    return fnindex.compare(index, obj, "AbsTest").ok


expect("check: same value", compare(name, value), True)
expect("check: other value", compare(name, value + 4), False)
expect("check: symbol not in the list", compare("data_unlisted", value), False)

sys.exit(1 if failures else 0)
