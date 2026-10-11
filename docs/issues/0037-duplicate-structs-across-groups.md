# 0037: Groups that work on the same object define separate structs for it

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-11
- **Milestone:** M2

## Problem

Agents of different groups, or of one batch, each declared their own struct for the same data:

- ov003: two `Ov003Object` structs on mainline, 14 bytes in `include/game/ov003_object.h` and 12 bytes in `ov003_effect.h`. A merge also dropped `Ov003Row.unk_36` (journal, `b12/o3m5`).
- ov005: four structs for one 0x97e0-byte work block (`b13/o5m2`).

`header_merge.py` merges by struct offset but not by meaning, so duplicates survive until someone finds them.

## Workaround

The renames pass merges them by hand.

## Ideas

- Queue a rename job per batch that lists struct pairs of equal size or with overlapping field offsets.
- Tell groups in the same overlay which header owns the shared object.
