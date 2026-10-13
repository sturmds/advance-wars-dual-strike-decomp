#!/usr/bin/env python3
"""Check where `fn.py add` ends a new delinks.txt range (issues 0051, 0063).

    python3 tools/tests/test_fnadd.py

The range ends at the function's size in symbols.txt, or at the next symbol
of any kind if that comes first, with padding to a 4-byte boundary when the
next symbol starts there. Unnamed bytes after the function stay out and are
reported. First on a made-up module, then `cmd_add` writing a delinks.txt in
a temporary directory, then the three cases from the issues on config/.
Needs no build and no ROM.
"""

import argparse
import contextlib
import io
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import fn  # noqa: E402

failed = False


def check(what: str, good: bool, detail="") -> None:
    global failed
    print(f"{what}: {'ok' if good else 'FAIL'}{f' ({detail})' if detail and not good else ''}")
    failed |= not good


# Real config first, before all_modules is replaced below.
real = {
    "NitroStartUp": 0x02000b00,              # BuildInfo follows (b26-crt0)
    "Reset_MasterOffAndReboot_Arm7": 0x03807424,  # data_arm7_03807424 follows (issue 0063)
    "Screen_IsTextBg": 0x01ff8f64,           # a table follows (issue 0051)
    "MTX_RotY43_": 0x021096d8,               # Thumb, two bytes of padding
    "RangeMap_FillMoves": 0x01ffb95c,        # gMap3dClearBlock follows (was an unnamed tail)
}
for name, want in real.items():
    got = fn.next_start(fn.find(name))
    check(f"{name} ends at 0x{want:08x}", got == want, hex(got))


# A made-up module. .text runs 0x1000-0x1100.
mod = fn.Module("main", Path("."))
mod.text = (0x1000, 0x1100)
funcs = [
    ("f_data_after", "arm", 0x1000, 0x10),     # data symbol right at its end
    ("f_gap", "arm", 0x1020, 0x10),            # 0x1030-0x1040 unnamed, then a function
    ("f_thumb_pad", "thumb", 0x1040, 0x0e),    # ends 0x104e, next function at 0x1050
    ("f_thumb_gap", "thumb", 0x1050, 0x0e),  # ends 0x105e, unnamed bytes until data at 0x1070
    ("f_label", "arm", 0x1080, 0x20),          # a label inside it, at 0x1088
    ("f_last", "arm", 0x10f0, 0x20),           # size runs past the .text end
]
for name, mode, addr, size in funcs:
    mod.funcs.append(fn.Func("main", name, mode, addr, size))
    mod.syms[addr] = name
mod.syms[0x1010] = "data_1010"
mod.syms[0x1070] = "data_1070"
mod.syms[0x1088] = "_label"  # labels are not in starts
mod.starts = sorted(a for a in mod.syms if a != 0x1088)
fn.all_modules = lambda: {"main": mod}


def f(name: str) -> fn.Func:
    return next(g for g in mod.funcs if g.name == name)


check("stops at a data symbol", fn.next_start(f("f_data_after")) == 0x1010)
check("no gap before a data symbol", fn.trailing_gap(f("f_data_after")) is None)
check("leaves an unnamed tail out", fn.next_start(f("f_gap")) == 0x1030)
check("reports the unnamed tail", fn.trailing_gap(f("f_gap")) == (0x1030, 0x1040))
check("takes in Thumb padding", fn.next_start(f("f_thumb_pad")) == 0x1050)
check("no gap after Thumb padding", fn.trailing_gap(f("f_thumb_pad")) is None)
check("no padding before unnamed bytes", fn.next_start(f("f_thumb_gap")) == 0x105e,
      hex(fn.next_start(f("f_thumb_gap"))))
check("reports bytes after a Thumb function", fn.trailing_gap(f("f_thumb_gap")) == (0x105e, 0x1070))
check("a label does not cut the range", fn.next_start(f("f_label")) == 0x10a0)
check("stops at the .text end", fn.next_start(f("f_last")) == 0x1100)

# cmd_add on a delinks.txt in a temporary directory.
with tempfile.TemporaryDirectory() as tmp:
    root = Path(tmp)
    mod.dir = root
    (root / "delinks.txt").write_text(
        "    .text       start:0x00001000 end:0x00001100 kind:code align:32\n\n"
        "src/a.c:\n    .text       start:0x00001080 end:0x000010a0\n")
    fn.ROOT = root
    err = io.StringIO()
    with contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(err):
        fn.cmd_add(argparse.Namespace(name="f_gap", path="src/game/main/f_gap.c"))
    text = (root / "delinks.txt").read_text()
    check("cmd_add writes the cut range",
          "src/game/main/f_gap.c:\n    .text       start:0x00001020 end:0x00001030" in text, text)
    check("cmd_add keeps entries sorted", text.index("f_gap.c") < text.index("src/a.c"), text)
    check("cmd_add prints the gap", "0x00001030-0x00001040" in err.getvalue(), err.getvalue())

sys.exit(1 if failed else 0)
