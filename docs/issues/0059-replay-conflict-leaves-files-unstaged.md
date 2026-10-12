# 0059: After a conflict in a replay, the resolved commit held one file and the next commit was dropped

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-11
- **Milestone:** M1

## Problem

During lcf-align2 (2026-10-11 17:17), a replay stopped on a conflict in `CLAUDE.md`. After the conflict was resolved by hand, the replayed commit held only the resolved file: the commit's other files were in the working tree but not staged. The next commit of the branch was then dropped as empty.

## Notes

What `tools/apply_commit.py` does on a conflict (read from the code, not reproduced):

- It writes every merged file to the working tree and collects conflicts in `CONFLICTS`. When the list is not empty it exits with `error: conflict in ...; resolve, then commit by hand` before the `git add` calls at the end of `main()`. So nothing is staged: conflicted and clean files are all unstaged working-tree changes.
- Whoever resolves the conflict, runs `git add` on the resolved file and commits, commits only that file. The clean files stay as unstaged edits.
- The next step of the replay then either meets the unstaged edits (`working tree has uncommitted changes`) or finds them already equal to what the next commit changes and reports that nothing is left to commit. That is how a commit can vanish. Which of the two happened was not checked.
- `--no-commit` stages everything, but the conflict path never gets there.

Fix idea: on a conflict, stage everything except the conflicted files and print the list, so that after the fix `git add` of the resolved files and `git commit -C` hold the whole commit. Add a case to `tools/tests/test_landing.py`: a conflict in one file with clean changes in two others, followed by a second commit.

## Done when

- That test ends with a commit holding all three files and a following commit still replayed.
