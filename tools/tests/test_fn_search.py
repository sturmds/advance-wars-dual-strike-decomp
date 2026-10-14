#!/usr/bin/env python3
"""Check `fn.py search ADDR` (tools/fn_search.py).

    python3 tools/tests/test_fn_search.py

The symbol, section and delinks.txt file come from config/ alone, so most
checks need no build and no ROM. The linked object, the original
instruction and the C line need a build; they are checked when one exists.
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import fn_search  # noqa: E402

failed = False


def check(what: str, good: bool, detail="") -> None:
    global failed
    print(f"{what}: {'ok' if good else 'FAIL'}{f' ({detail})' if detail and not good else ''}")
    failed |= not good


def search(addr: int, module: str | None = None, lines: bool = False) -> str:
    return "\n".join(fn_search.search(addr, module, lines))


check("address forms", {fn_search.parse_addr(t) for t in ("0x20D7164", "020d7164", "0x020d7164")} == {0x020d7164})
try:
    fn_search.parse_addr("DesignMap_UnpackRecord")
    check("a name is not an address", False)
except ValueError:
    check("a name is not an address", True)

out = search(0x020d7164)
check("main code", out.startswith("0x020d7164  main  .text (code)"), out)
check("function and offset", "DesignMap_UnpackRecord+0x78  (function arm at 0x020d70ec, size 0x190)" in out, out)
check("delinks file", "file:    src/game/main/map/design_map_unpack_record.c" in out, out)

# Overlays 1, 2, 3, 5 and 6 load at 0x02350560: one answer each.
out = search(0x02360000)
mods = [line.split()[1] for line in out.splitlines() if line.startswith("0x")]
check("every overlay at a shared address", mods == ["ov001", "ov002", "ov003", "ov005", "ov006"], mods)
out = search(0x02360000, "ov002")
check("--module picks one overlay", out.count("\n0x") == 0 and " ov002 " in out, out)
check("--module names the others", search(0x020d7164, "ov002").endswith("not in module ov002; in main"))

out = search(0x0216e030)
check("bss with its size", "gOverlayLoaded+0x10  (bss at 0x0216e020, size 0x1c)" in out, out)
check("bss declaration", "u32 gOverlayLoaded[" in out, out)

# ARM9 dtcm and ARM7's main-RAM part both start at 0x027c0000.
out = search(0x027c0014)
check("dtcm and arm7 share an address", "  dtcm  .data (data)" in out and "  arm7  .text (code)" in out, out)
check("ARM7 code", "Wmsp_TakeFlaggedSlot+0x14" in out, out)

check("I/O register", search(0x04000208) == "0x04000208  not in any module; hardware I/O registers")

if fn_search.MAPS["arm9"].exists() and (fn_search.fn.ROOT / "build/delinks").exists():
    out = search(0x020d7164, lines=True)
    check("linked object", "linked:  build/src/game/main/map/design_map_unpack_record.o" in out, out)
    check("original instruction", "insn:    78: strb\tr3, [r2, #141]" in out, out)
    check("C line", "C line:  src/game/main/map/design_map_unpack_record.c:" in out, out)
    out = search(0x027c0014)
    check("ARM7 linked object", "build/arm7/src/arm7/sdk/wm/wmsp_take_flagged_slot.o" in out, out)
else:
    print("no build: linked object, instruction and C line not checked (run make)")
sys.exit(1 if failed else 0)
