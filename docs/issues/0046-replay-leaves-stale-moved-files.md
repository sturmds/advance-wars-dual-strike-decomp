# 0046: replay leaves the old copy of a moved source file

- **Status:** closed
- **Area:** loop
- **Opened:** 2026-10-11
- **Milestone:** M1

## Problem

`PROGRESS.md` listed 10 files as "no delinks entry". Each one was a stale copy of a function that is matched in a file under another path:

- `main/func_0200a6c0.c`, `func_02067b94.c`, `func_02067bec.c`, `func_020d8a6c.c`, `func_020f6728.c`, `func_020f678c.c`, `func_020f67f0.c`, `func_020f6854.c`
- `ov000/player_hud_draw_slot.c`, `ov006/func_ov006_0235e04c.c`

Two ways it happened, seen in the history:

- A branch moved a file (delete old, add new). `merge_file` in `tools/apply_commit.py` deletes the old file only when it still equals the commit's base text. If mainline had edited it meanwhile (for example a symbol rename applied by the replay), the deletion is skipped without a message. `4516d8821` (map view row and column files) shows this.
- A commit that only edited the old path (a rename pass over all `src/`) made `merge_file` write the file again, because it writes `theirs` when the target is missing. `41bd7c4e9` (VsSetup_ClearPlayerReady/SetPlayerReady) shows this.

The stale files had no `delinks.txt` entry, so the build ignored them, but two files defined each function.

## Workaround

Removed the ten files. Check the "no delinks entry" rows of `PROGRESS.md` after each batch.

## Ideas

- In `merge_file`, when the commit deletes a file and ours differs from base, apply the symbol renames to base first and compare, or delete anyway when the difference is only renames.
- When the target is missing and the commit's version is a modification (base is not None), report a conflict instead of recreating the file.
- Have `land_self.py` fail when a `src/` file has no `delinks.txt` entry and another source file defines the same function.

## Fixed (2026-10-11)

`merge_file` in `tools/apply_commit.py`:

- When the commit deletes a file, it applies the commit's renames to both sides and deletes the file when this branch's copy equals the parent's apart from identifiers (`names_only_differ`), so renames landed since no longer keep the old copy. Any other change is a conflict that says so, not a silent skip.
- When the commit edits a file this branch no longer has, it skips the edit if it only applies the commit's symbol renames (`rename_in_sources` gives the moved file the same renames). Otherwise it reports a conflict naming the likely new path. It never writes the old path again.

Cases in `tools/tests/test_landing.py` (`MovedFileTest`). The third idea (a `land_self.py` check for duplicate definitions) was not needed for these two causes.
