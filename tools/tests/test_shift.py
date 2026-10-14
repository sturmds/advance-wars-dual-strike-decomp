#!/usr/bin/env python3
"""Check the parts of tools/shift_build.py that need no build.

    python3 tools/tests/test_shift.py

- moving_from follows `ORIGIN = AFTER(...)` in dsd's MEMORY block.
- shift_of adds one pad per grown module whose growth moves the symbol.
- write_lcf puts a pad before an object's line or after a `NAME = .;` line.
"""

import os
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import shift_build  # noqa: E402

failures = []


def expect(what: str, got, want) -> None:
    print(f"{what}: {'ok' if got == want else 'FAIL'}")
    if got != want:
        failures.append(what)


MEMORY = """MEMORY {
    ARM9 : ORIGIN = 0x02000000 > build/arm9.bin
    ITCM : ORIGIN = 0x01ff8000 > build/itcm.bin
    OV000 : ORIGIN = AFTER(ARM9) > build/arm9_ov000.bin
    OV004 : ORIGIN = AFTER(ARM9) > build/arm9_ov004.bin
    OV001 : ORIGIN = AFTER(OV000) > build/arm9_ov001.bin

    SPACE : ORIGIN = AFTER(ARM9, OV000, OV004, OV001)
}
"""

expect("moving: main moves every overlay", shift_build.moving_from("main", MEMORY),
       {"ARM9", "OV000", "OV004", "OV001"})
expect("moving: ov000 moves ov001, not ov004", shift_build.moving_from("ov000", MEMORY), {"OV000", "OV001"})
expect("moving: itcm alone", shift_build.moving_from("itcm", MEMORY), {"ITCM"})

shift_build.moving_regions = lambda m: shift_build.moving_from(m, MEMORY)
pads = [("main", 0x02004000), ("ov000", 0)]
expect("shift_of: main before the pad", shift_build.shift_of(0x02000800, "ARM9", pads, 0x40), 0)
expect("shift_of: main after the pad", shift_build.shift_of(0x02004000, "ARM9", pads, 0x40), 0x40)
expect("shift_of: ov000 moves twice", shift_build.shift_of(0x022ad560, "OV000", pads, 0x40), 0x80)
expect("shift_of: ov004 moves with main only", shift_build.shift_of(0x022ad560, "OV004", pads, 0x40), 0x40)
expect("shift_of: itcm stays", shift_build.shift_of(0x01ff8000, "ITCM", pads, 0x40), 0)
expect("shift_of: a script constant outside the region's range stays",
       shift_build.shift_of(0x00000001, "ARM9", pads, 0x40), 0)

with tempfile.TemporaryDirectory() as d:
    lcf = "    .ov000 : {\n        OV000_TEXT_START = .;\n        a.o(.text)\n        b.o(.text)\n    }\n"
    old = Path.cwd()
    try:
        os.chdir(d)
        Path("build").mkdir()
        Path("build/arm9.lcf").write_text(lcf)
        out = shift_build.write_lcf(Path("."), ["        OV000_TEXT_START = .;\n", "        b.o(.text)\n"], 0x20)
        expect("write_lcf: after a label, before an object", out.read_text(),
               "    .ov000 : {\n        OV000_TEXT_START = .;\n        . = . + 0x20;\n        a.o(.text)\n"
               "        . = . + 0x20;\n        b.o(.text)\n    }\n")
    finally:
        os.chdir(old)

sys.exit(1 if failures else 0)
