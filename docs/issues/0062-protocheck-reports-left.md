# 0062: About 190 `protocheck.py` reports left in files touched only by renames

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-11
- **Milestone:** M2

## Problem

After renames batch 27 (48e8934a7), `python3 tools/protocheck.py --all` still reports about 190 declarations or calls that disagree with the callee's definition. They were left because changing a prototype or the width of an argument can change how the caller narrows a value, and move a match.

## Notes

Main groups, from the renames27 results:

- About 30 callers declare `Wireless_GetSharedData` as `int(int)` while the definition takes `u16`.
- `func_0207b4b0`: `int` against a pointer.
- The OS arena getters and setters: the definitions use a placeholder struct and mixed `int` and `void *` types.
- `MI_CpuFill8` and `Mem_*`: argument widths.
- `func_0211b458`: `void` against `int`.
- `MbParent_CommCallback`: `u16` against `u32`.
- `func_02121ea8` (a fifth trap wrapper) declared `double` in `dfix.c`.
- `DestructorChain.destructor` stays one-argument: `Overlay_RunDestructorsInRange` calls it with `r1` still holding the function pointer, so a two-argument type would break that match. `__destroy_global_chain` keeps its cast.

Other leftovers of the same pass, not protocheck reports: the `MbBeacon_Checksum` prototype in `MbBeacon_SendPiece`; the `data_022ace48` ggid callback type; `DlPlayStationArea` 0x0c-0x16 as `WMParentParam` fields; `DlPlayBlockSpot.unk_00` as `childAddress`; `OSContext.cpContext` as `CPContext`; `Touch_*` prototypes as `TPData`; the extern `data_022aced4` still declared in each MB file.

Method: change one declaration, run `fn.py diff` on the file and on every file that includes the changed header, and keep the change only if no score drops. Reports of kind `pointer` do not change code and can go first.

## Done when

- `protocheck.py --all` reports nothing, or each remaining report has a comment saying why it stays.
