#!/usr/bin/env python3
"""Check tools/arm7.py without a ROM.

    python3 tools/tests/test_arm7.py

1. A made-up arm7.bin with the AWRE layout splits into the three parts.
2. ARM BL, ARM BLX and Thumb BL fields decode to their targets, and encode
   back to the same word.
3. Moving a branch and a pointer to the stand-in addresses and reading them
   back there gives the moved targets.
4. The stand-in config puts each line in the right part, with addresses
   moved and `module:arm7` turned into dsd's autoload index.
"""

import struct
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import arm7  # noqa: E402

failures = []


def check(what: str, ok: bool, detail=""):
    print(f"{what}: {'ok' if ok else 'FAIL'}{f' ({detail})' if detail and not ok else ''}")
    if not ok:
        failures.append(what)


def fake_arm7() -> bytes:
    """crt0 of 0x170 bytes, wram part of 0x20, main part of 0x40, table."""
    static = bytearray(0x170)
    list_start = arm7.STATIC_BASE + 0x170 + 0x20 + 0x40
    struct.pack_into("<5I", static, arm7.PARAMS, list_start, list_start + 24,
                     arm7.STATIC_BASE + 0x170, arm7.STATIC_BASE + 0x170, arm7.STATIC_BASE + 0x170)
    table = struct.pack("<6I", 0x037f8000, 0x20, 0x10, 0x027c0000, 0x40, 0x8)
    return bytes(static) + bytes(0x20) + bytes(0x40) + table


def main() -> int:
    regs = arm7.regions(fake_arm7())
    got = [(r.name, r.base, r.size, r.bss, r.offset) for r in regs]
    want = [("static", 0x02380000, 0x170, 0, 0), ("wram", 0x037f8000, 0x20, 0x10, 0x170),
            ("main", 0x027c0000, 0x40, 0x8, 0x190)]
    check("regions", got == want, got)
    wram = regs[1]
    check("wram end includes aligned .bss", wram.end == 0x037f8020 + 0x10, hex(wram.end))

    cases = [
        ("arm_call", 0xeb000000, 0x027c0100, 0x037f8040),
        ("arm_call", 0x0b000000, 0x037f8040, 0x027c0100),   # conditional BL backwards
        ("arm_call_thumb", 0xfa000000, 0x027c0100, 0x037f8042),
        ("thumb_call", 0xf800f000, 0x037f8002, 0x03800104),     # Thumb BL reaches +-4 MB
        ("thumb_call_arm", 0xe800f000, 0x037f8002, 0x037f0104),
    ]
    for kind, base_word, pc, target in cases:
        word = arm7.encode_branch(base_word, pc, target, kind)
        back = arm7.branch_target(word, pc, kind)
        check(f"{kind} 0x{pc:08x} -> 0x{target:08x}", back == target, f"got 0x{back:08x}")
        check(f"{kind} keeps the opcode bits", arm7.encode_branch(word, pc, target, kind) == word)

    try:
        arm7.encode_branch(0xf800f000, 0x037f8002, 0x027c0104, "thumb_call")
        check("Thumb BL out of range is refused", False)
    except ValueError:
        check("Thumb BL out of range is refused", True)

    word = arm7.encode_branch(0xeb000000, 0x027c0100, 0x037f8040, "arm_call")
    moved = arm7.moved_word(regs, word, 0x027c0100, "arm_call", 0x037f8040)
    check("moved BL reaches the moved target",
          arm7.branch_target(moved, arm7.to_fake(regs, 0x027c0100), "arm_call") == arm7.to_fake(regs, 0x037f8040))
    moved = arm7.moved_word(regs, 0x037f8011, 0x027c0000, "load", 0x037f8010)
    check("moved pointer keeps the Thumb bit", moved == arm7.to_fake(regs, 0x037f8010) | 1, hex(moved))
    check("to_real undoes to_fake", all(arm7.to_real(regs, arm7.to_fake(regs, a)) == a
                                        for a in (0x02380010, 0x037f8000, 0x037f8030, 0x027c0044)))

    cfg = arm7.Config(
        sections=["    .text       start:0x02380000 end:0x02380170 kind:code align:32",
                  "    .text       start:0x027c0000 end:0x027c0040 kind:code align:32",
                  "    .text       start:0x037f8000 end:0x037f8020 kind:code align:32"],
        files=[["src/arm7/sdk/a.c:", "    complete", "    .text       start:0x037f8000 end:0x037f8010"]],
        symbols=["func_arm7_037f8000 kind:function(arm,size=0x10) addr:0x037f8000",
                 "func_arm7_027c0000 kind:function(arm,size=0x40) addr:0x027c0000"],
        relocs=["from:0x027c0010 kind:arm_call to:0x037f8000 module:arm7"])
    data = fake_arm7()
    parts = {r.name: data[r.offset:r.offset + r.size] for r in regs}
    with tempfile.TemporaryDirectory() as tmp:
        out = Path(tmp)
        arm7.fake_config(cfg, regs, out, parts)
        wram_delinks = (out / "autoload_3/delinks.txt").read_text()
        check("file block goes to the wram part", "src/arm7/sdk/a.c:\n    complete\n" in wram_delinks)
        check("file block addresses moved", "start:0x02a00000 end:0x02a00010" in wram_delinks)
        relocs = (out / "autoload_4/relocs.txt").read_text()
        check("relocation goes to the main part, module index set",
              relocs == "from:0x02c00010 kind:arm_call to:0x02a00000 module:autoload(3)\n", relocs)
        check("config lists three unknown autoloads",
              (out / "config.yaml").read_text().count("Unknown:") == 3)

    print(f"\n{len(failures)} failure(s)" if failures else "\nall ok")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
