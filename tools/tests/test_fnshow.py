#!/usr/bin/env python3
"""Check the `refs:` line of `fn.py show` (tools/fn.py refs_to).

    python3 tools/tests/test_fnshow.py

The function at main 0x0201d320 has no direct caller; one entry of the table
at main 0x021585c0 points at it (issue 0003). Then the ARM7 module: found by
name, kept out of fn.modules() (loop_state.py counts on that), callers
across ARM7's parts, and the end of a file taken from the .text section of
the function's own part. Reads config/ only: no build and no ROM needed.
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import fn  # noqa: E402

f = fn.find("0x0201d320")
table = fn.modules()["main"].syms[0x021585c0]
refs = fn.refs_to(f)
ok = f"{table}+0x14c (main)" in refs and not fn.callers_of(f)
print(f"refs_to {f.name}: {'ok' if ok else 'FAIL'} ({'; '.join(refs) or 'none'})")
failed = not ok


def check(what: str, good: bool, detail="") -> None:
    global failed
    print(f"{what}: {'ok' if good else 'FAIL'}{f' ({detail})' if detail and not good else ''}")
    failed |= not good


g = fn.find("Wmsp_SendReply")
check("ARM7 function found", g.module == "arm7" and g.addr == 0x038076e8, g)
check("fn.modules() stays ARM9 only", "arm7" not in fn.modules() and "arm7" in fn.all_modules())
check("caller in main RAM of a WRAM function", "Wmsp_ReplyFailed" in fn.callers_of(g))
# The last function of WRAM's .text ends the section, not at the next
# function in another part.
check("next_start stops at the WRAM .text end", fn.next_start(g) == 0x0380773c, hex(fn.next_start(g)))
sys.exit(1 if failed else 0)
