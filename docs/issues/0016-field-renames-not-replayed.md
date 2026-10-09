# 0016: `apply_commit.py` does not replay struct field renames

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-08
- **Milestone:** M1

## Problem

`apply_commit.py` replays symbol renames by address, but a struct field rename is an ordinary header edit. In batch 3, one agent renamed `Unk_02080440_48.unk_02` to `currentPlayer`; seven commits from other agents still used `unk_02`, and each failed `make release` when replayed. They were fixed by hand on `mainline`.

## Notes

- Option A: when a replayed commit fails to build, look for `unk_XX` fields that a header on `mainline` has since renamed (same offset, same struct) and substitute.
- Option B: tell agents not to rename fields in shared headers; collect field names in NOTES and apply them between batches.
- Option B is simpler and also avoids the duplicate edits seen in `player.h` (issue 0002).
- 2026-10-08: partly covered. When a commit's header conflicts with mainline, `tools/header_merge.py` matches fields by offset, and `apply_commit.py` rewrites the commit's `->old` and `.old` accesses to mainline's names. A commit that uses a renamed field without touching the header is still not rewritten: matching `unk_XX` by name alone would also hit other structs' fields.

## Done when

A field rename in one agent's commit no longer breaks other commits from the same batch.
