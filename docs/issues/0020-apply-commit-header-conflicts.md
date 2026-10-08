# 0020: `apply_commit.py` stops at shared-header conflicts

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-08
- **Milestone:** M1

## Problem

In batch 4, almost every agent added lines to `include/game/skills.h` and several filled padding in the same struct in `include/game/Unk_02080440.h`. `apply_commit.py` then:

- reports a conflict on the header and stops before writing the commit's other files, so new C files, other header edits and symbol renames in `src/` are missing;
- for padding, a line merge produces two `pad_59` members, which does not compile.

Each case was fixed by hand with helper scripts kept outside the repo (`work/loop-prompts/`): `union.py` (keep both sides' lines, sort `SKILL_` ids), `struct_merge.py` (merge two versions of a struct by field offset and regenerate padding), `finish_conflict.py` (finish the rest of the commit), `land.sh` (replay a list of commits).

## Notes

- Move the struct merge and the "finish the rest" step into `apply_commit.py`.
- `apply_commit.py` also adds a second `delinks.txt` entry when a commit renames a file it added earlier, instead of moving the entry.

## Done when

A batch whose agents all add fields and ids to the same headers lands with no hand edits.
