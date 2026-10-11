# 0019: Relocations marked `overlays(0,4)` that can only be ov000

- **Status:** closed
- **Area:** tools
- **Opened:** 2026-10-08
- **Milestone:** M3

## Problem

ov000 and ov004 load at the same address (`0x022ad560`). dsd marks 804 relocations in main `module:overlays(0,4)` because it cannot tell which one a pointer means. ov004 ends at `0x022bf140`, so every target above that is in ov000: the map header table (`Ov000_gMapHeaders`), unit stats (`Ov000_gUnitStats`), terrain and movement tables and text all sit there. The matching build does not care, but a shifted build (issue 0012) would resolve these wrong.

Resolved 2026-10-11 by `tools/resolve_overlay_relocs.py` (run once with `--write`; the docstring has the rules). All 832 `overlays(0,4)` relocations (804 in main, 10 in itcm, 15 in ov001, 3 in ov006) now say `overlay(0)`. The same rules decided 162 of the 177 relocations among ov001-ov006; the 15 left are in issue 0055.

## Notes

- The issue's premise was half right. ov004's code and data end at `0x022bf140`, but its .bss (zeroed memory with no bytes in the ROM) runs to `0x02353960` and covers all of ov000's .rodata, .data and .bss. ov004's own code only points at the starts of its buffers (`0x022bf140`-`0x022bf15c`, `0x022c395c`, `0x022d395c`), so a pointer from main into the middle of that .bss means ov000. Rule `bss-only` in the script.
- Below `0x022bf140` there were 4 relocations, all `bl 0x022ad560` from `UnloadTask_PickTile` and `UnitMoveSelect_TryMoveTo`. Both overlays have a function there; the callers walk unit paths, and ov000's is `UnitPath_CountSteps` (ov004's draws a textured quad). Decided by hand in the script's `DECIDED` table.
- Before the change, dsd named the first candidate's symbol in the delinked object, so the 0/4 relocations already linked to ov000. The ones among ov001-ov006 did not: 96 entries of a table in main (`data_0216b360` on, 64-byte stride) pointed at `data_ov001_02367eac` (ov001 .data) where ov005 has `func_ov005_02367eac`. Rule `function` fixed those.
- symbols.txt: 159 overlay symbols lost `ambiguous`, and 190 dsd-named shadow symbols that only existed for these relocations (most were `data_ov004_*` .bss) were removed. `make release` matches.

- A small script can rewrite the `module:` field of those lines in `config/AWRE/arm9/relocs.txt` to `overlay(0)` when the target is past ov004's end, then `make release` checks it.
- The ov000 symbols near these targets are marked `ambiguous` in `symbols.txt` for the same reason.

## Done when

No relocation with a target above ov004's end is marked `overlays(0,4)`.
