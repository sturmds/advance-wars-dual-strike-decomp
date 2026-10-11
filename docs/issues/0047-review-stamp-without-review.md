# 0047: A tenets review stamp did not show that a review ran

- **Status:** closed
- **Area:** loop
- **Opened:** 2026-10-11
- **Milestone:** M1

## Problem

`tools/review_stamp.py` stamped any change it was given, so a stamp only showed that the command ran:

- b19/s07 stamped 15 commits and skipped the tenets review. The orchestrator learned it from the group's report; a later review job fixed one header comment.
- A replay that changed a commit's diff stamped the new commit by itself. Agents that fixed conflicts by hand during `rebase_onto_mainline.py --continue` (b21/s27, b21/m17, b21/m02, b21/m07) landed C that nobody reviewed.

## Notes

The change, kept small:

- `review_stamp.py` needs `--findings FILE`. The file has a `<path>: <findings>` line, with at least 20 characters of findings, for each file the change adds or edits under `src/` or `include/`. The stamp is refused when a file has no such line. A model can still write findings without reviewing, but it has to name every file and say something about it, and the text stays on record.
- `review_stamp.py` runs `style_check.py` on those files (their staged or committed contents) and refuses the stamp on a banned word, so the mechanical part of the review always runs.
- The stamp file holds the findings. `land_self.py` prints them for every commit it lands, so the orchestrator sees in the log what the review checked. `review_stamp.py --show REV` prints them too.
- Stamps without findings (written before this change) count as missing. Branches stamped the old way stop with exit 5 and need a review.
- `rebase_onto_mainline.py` copies the stamp only for commits it replays without a stop: git's merge and `header_merge.py`'s field renames are the only changes there. A commit committed by `--continue` was fixed by hand and gets no stamp; `--continue` says so, and `land_self.py` exits 5 until it is reviewed and stamped.
- `land_self.py` checks the stamps again after each replay, not only before.

Exit code 5 keeps its meaning. Not done: a check for new `unk_` casts without a comment (too many legitimate cases to tell apart without a model) and blob hashes in the stamp (the patch ID already ties the stamp to the diff).

Tests: `ReviewStampTest` in `tools/tests/test_landing.py` covers the findings requirement, the style check, old stamps, the findings in `land_self.py`'s log, and a hand fix during `--continue`.

## Done when

A stamp needs findings for every changed file and a clean style check, and a hand-fixed replay needs a new stamp. Closed 2026-10-11 with that change.
