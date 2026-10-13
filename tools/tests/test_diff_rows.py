#!/usr/bin/env python3
"""Check the rows fn.py diff marks `=` for issue 0077.

    python3 tools/tests/test_diff_rows.py

- branch_target: a branch with a relocation and one with only a
  destination match when they reach the same address. A `b` that dsd wrote
  over a conditional branch matches our `bmi` only when the ROM word has
  that condition.
- resolved_word: a plain word matches a word relocated to a symbol of that
  value.
- same_bytes: the same bytes match however objdiff decodes them; other
  bytes, or bytes under a relocation, do not.

Rows are shaped like objdiff-cli's JSON. Symbol addresses and ROM words are
stubbed, so no ROM or build is needed.
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import fn  # noqa: E402

failures = []


def expect(what: str, got, want) -> None:
    print(f"{what}: {'ok' if bool(got) == want else 'FAIL'}")
    if bool(got) != want:
        failures.append(what)


FUNC, NEXT = 0x02000100, 0x02000110
fn.symbol_addresses = lambda: {"Func": FUNC, "Next": NEXT, "Data": 0x02000208}
rom = {}
fn.rom_word = lambda module, addr: rom.get(addr)


def branch(mnemonic: str, dest: int | None = None, reloc: str | None = None, address: int = 4,
           kind: str = "DIFF_ARG_MISMATCH") -> dict:
    ins = {"address": str(address), "size": 4,
           "parts": [{"opcode": {"mnemonic": mnemonic, "opcode": 32772}},
                     {"arg": {"reloc": True} if reloc else {"branch_dest": str(dest)}}]}
    if reloc:
        ins["relocation"] = {"type": 1, "type_name": "R_ARM_PC24", "target_symbol": 1, "addend": "-8"}
    return {"diff_kind": kind, "instruction": ins}


# Func sits at offset 0x20 in the original's object and at 0 in ours.
data = {"left": {"symbols": [{"name": "Func", "address": "32"}, {"name": "Next"}]},
        "right": {"symbols": [{"name": "Func"}, {"name": "Next"}]}}
expect("branch: same address", fn.branch_target(
    data, branch("b", dest=0x30, address=0x24), branch("b", reloc="Next"), "Func"), True)
expect("branch: other address", fn.branch_target(
    data, branch("b", dest=0x34, address=0x24), branch("b", reloc="Next"), "Func"), False)
expect("branch: other condition", fn.branch_target(
    data, branch("beq", dest=0x30, address=0x24), branch("bne", reloc="Next"), "Func"), False)

op = "DIFF_OP_MISMATCH"
rom[FUNC + 4] = 0x4A000001  # bmi
expect("dsd's b, ROM has our bmi", fn.branch_target(
    data, branch("b", reloc="Next", address=0x24, kind=op), branch("bmi", reloc="Next", kind=op), "Func"), True)
rom[FUNC + 4] = 0x5A000001  # bpl
expect("dsd's b, ROM has bpl", fn.branch_target(
    data, branch("b", reloc="Next", address=0x24, kind=op), branch("bmi", reloc="Next", kind=op), "Func"), False)
expect("b with no relocation against bmi", fn.branch_target(
    data, branch("b", dest=0x30, address=0x24, kind=op), branch("bmi", reloc="Next", kind=op), "Func"), False)
rom.clear()
expect("dsd's b, no ROM", fn.branch_target(
    data, branch("b", reloc="Next", address=0x24, kind=op), branch("bmi", reloc="Next", kind=op), "Func"), False)

word = {"opcode": {"mnemonic": ".word", "opcode": 65534}}


def words(value: int) -> tuple[dict, dict, dict]:
    left = {"diff_kind": "DIFF_ARG_MISMATCH", "instruction": {"parts": [word, {"arg": {"unsigned": str(value)}}]}}
    right = {"diff_kind": "DIFF_ARG_MISMATCH",
             "instruction": {"parts": [word, {"arg": {"reloc": True}}],
                             "relocation": {"type": 2, "type_name": "R_ARM_ABS32", "target_symbol": 0,
                                            "addend": "8"}}}
    return {"right": {"symbols": [{"name": "Func"}]}}, left, right


expect("word: symbol plus addend", fn.resolved_word(*words(FUNC + 8)), True)
expect("word: other value", fn.resolved_word(*words(FUNC + 12)), False)


def row(address: int, size: int, kind: str = "DIFF_REPLACE") -> dict:
    return {"diff_kind": kind, "instruction": {"address": str(address), "size": size, "parts": []}}


code = bytes(range(16))
orig = (bytes(8) + code, 8, set())  # Func at offset 8
ours = (code, 0, set())
expect("bytes: same word", fn.same_bytes(row(8 + 4, 4), row(4, 4), (orig, ours)), True)
expect("bytes: Thumb halfwords of one word", fn.same_bytes(row(8 + 4, 4), row(4, 2), (orig, ours)), True)
expect("bytes: row only ours has", fn.same_bytes({"diff_kind": "DIFF_INSERT"}, row(12, 4, "DIFF_INSERT"),
                                                 (orig, ours)), True)
changed = (bytes(8) + code[:4] + b"\xff" + code[5:], 8, set())
expect("bytes: other word", fn.same_bytes(row(8 + 4, 4), row(4, 4), (changed, ours)), False)
expect("bytes: past the other section", fn.same_bytes(row(8 + 12, 4), row(16, 4), (orig, ours)), False)
expect("bytes: relocation in ours", fn.same_bytes(row(8 + 4, 4), row(4, 4), (orig, (code, 0, {4}))), False)
expect("bytes: no objects", fn.same_bytes(row(8 + 4, 4), row(4, 4), None), False)

if failures:
    sys.exit(f"test_diff_rows: {len(failures)} failed: {', '.join(failures)}")
print("test_diff_rows: all passed")
