# 0029: header_merge.py placed the commit's fields with mainline's struct sizes

- **Status:** closed
- **Area:** loop
- **Opened:** 2026-10-10
- **Milestone:** M1

## Problem

Batch 12, groups o4m2 and o4s2 (landed 7277be829): replays of `include/game/Unk_ov004_022bf158.h` reported renames that were not renames (`unk_8f8->unk_8fc`, `unk_8fc->unk_900`, `unk_900->unk_904`, later `blocksWide->unk_1338`), then stopped on `slotTask (0xa18) overlaps mainline's slotTask (0xa1c)` and on the "field also exists in another struct" check. The agents fixed the header by hand.

Cause: `Types` keeps the first definition it reads of each struct, and `apply_commit.py` gave it mainline's header first. Mainline had grown `Unk_ov004_022bf158_818` from 0x10 to 0x14 bytes; the commit still had 0x10. The commit's fields after the embedded `unk_818` were laid out with mainline's size, so every field after it (`unk_8f8`, `slotTask`, `blocksWide`, ...) was placed 4 bytes late.

## Fixed (2026-10-10)

- `header_merge.merge()` lays out each version with its own types: mainline's fields with mainline's definitions first, the commit's with the commit's, the base's with the base's (`Sides`, `Types.first`).
- `apply_commit.header_types()` returns both sets: mainline's include/ files first, or the commit's version of the header and of the other headers it changed first.
- Replaying the o4s2 chain (cc7cfe8c4 onto d2604b024) now gives no renames and no conflicts, and the same header the agent wrote by hand.

Still open: the merged body is checked with mainline's types. If the merge itself changes the size of an embedded struct, the fields after it in the other struct are not moved.

Tests: `make test` (`EmbeddedStructSizeTest` in `tools/tests/test_landing.py`).
