# 0011: `pre-commit` builds the working tree, not the staged changes

- **Status:** open
- **Area:** build
- **Opened:** 2026-10-07
- **Milestone:** later

## Problem

`hooks/pre-commit` runs `make release` on the working tree. If some changes are unstaged, the hook can pass for a tree that differs from what gets committed. D4 says every commit passes `make release`.

## Notes

- So far every commit was made with a clean tree, and agents stage everything they changed, so it has not mattered.
- Fix: in the hook, build from a temporary checkout of the index (`git checkout-index -a --prefix=...`) reusing `.cache/` and `orig/`. Costs a full rebuild per commit (about 5 s today).

## Done when

A commit with unrelated unstaged changes is still checked against exactly what is staged.
