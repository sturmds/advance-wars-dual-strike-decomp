# 0028: header_merge.py crashed on bit-fields named pad_* and dropped anonymous struct members

- **Status:** closed
- **Area:** loop
- **Opened:** 2026-10-10
- **Milestone:** M1

## Problem

Batch 12, group o3s2 (landed f7c52a696): the replay stopped with `TypeError` at `last.name += ...` in `layout()`. Both sides had laid anonymous unions over the same fields (`Ov003State.unk_1d6` with a struct for `unk_1d7`, an `s16 frame` view of `Unk_ov003_0235dd38.unk_4c`), and the commit's version had bit-fields. The agent merged the header by hand.

Two faults in `tools/header_merge.py`:

- `layout()` turns every field named `pad*` into padding (name `None`). It did this for bit-fields too, so a bit-field after a `pad_*` bit-field in the same storage unit was joined to an item with no name, and `last.name += ...` raised.
- `merge_union()` merged members by name only. When both sides had an anonymous struct member (`@0`) with different fields, mainline's stayed and the commit's new fields were dropped without a note.

## Fixed (2026-10-10)

- A bit-field named `pad*` stays a field; only whole fields named `pad*` are padding.
- In a union, two versions of an anonymous struct or union member are merged field by field (`merge_nested`), as they already were directly inside a struct. A named member both sides changed keeps mainline's version with a "kept mainline's" note.
- The "took the commit's rename" note is no longer printed for anonymous members (`@2`).

Tests: `make test` (`AnonymousUnionTest` in `tools/tests/test_landing.py`).
