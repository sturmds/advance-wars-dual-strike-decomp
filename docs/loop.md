# Automated decompilation loop

How the top-level Claude session runs batches of agents, each matching one function, and lands their work on `mainline`. The per-function procedure is in `.claude/skills/decomp-function/SKILL.md`; the manual version is `docs/workflow.md`.

## One batch

1. **Pick.** `python3 tools/fn.py pick -n 40` lists functions not yet in any file: entries in `config/AWRE/interesting.txt` first, then fewest unmatched callees, then smallest. Choose a batch (5–10 at first) and skip functions that call each other, so two agents never need the same callee renamed.
2. **Start agents.** One background agent per function, each with `isolation: worktree` (the worktree appears under `.claude/worktrees/`, which git ignores). The prompt names the function and points at the skill. Each agent runs `tools/wt_setup.sh` (links `orig/` and `.cache/`), works, and either commits on its branch or stops.
3. **Collect.** Each agent ends with a `RESULT:` block: `matched` with a commit hash, or `partial`/`skipped` with notes.
4. **Land.** For each matched commit, in any order, on `mainline`:
   ```sh
   python3 tools/apply_commit.py <hash>
   ```
   It replays the commit by meaning (delinks entries by address, symbol renames by address, source files with a 3-way merge) and commits with the original message; the hooks run `make release`. On a conflict it stops with the changes staged.
5. **Clean up.** `git worktree remove` each finished worktree and delete its branch. Partial attempts can be kept for a second pass: note the function and best percentage in the batch log below.

## Rules for the orchestrator

- Nothing lands unless `make release` passes on `mainline` after the replay.
- Review each landed commit's C against `TENETS.md` before the next batch; fix the skill when the same problem shows up twice.
- Agents do not edit `tools/`, `docs/`, `Makefile` or `hooks/`. Tool fixes happen on `mainline` between batches.

## Batch log

| Date | Functions | Matched | Notes |
|---|---|---|---|
| 2026-10-06 pilot | func_02003f6c, func_0200a7b0, func_02080440, func_ov003_023517e4, func_02066818 | 5/5 | All matched in 1–2 rounds, about 1.5 min and 50k tokens each. Fixes from feedback: `fn.py finish` (sandbox refuses the word "complete"), one command per call, no shared scratch files, `.gitignore` for symlinks, trailer-aware style check, `show` on large functions. |
