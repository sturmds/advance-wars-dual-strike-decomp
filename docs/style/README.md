# Writing style lists

Used by `tools/style_check.py`, the `commit-msg` hook and the review pass.

| File | Effect |
|---|---|
| `banned.txt` | Hard bans. `tools/style_check.py` exits non-zero, and the `commit-msg` hook refuses the commit. |
| `claudeisms.csv` | Warnings. Words a model overuses. Many are normal here (`diff`, `sha`, `pinned`), so the review pass decides case by case. |

Also banned, and checked by a heuristic that only warns: the aphorism "X is the Y" ("the hash is the test", "the linker script is the map"). Say what the thing does instead: "`make release` fails if any byte differs."

## Source of `claudeisms.csv`

Copied unchanged from [archiewood/claudeisms](https://github.com/archiewood/claudeisms), commit `49f625b657edcdba8e43fb2857975d9d50c72d96` (2026-07-16), fetched 2026-10-06. SHA-256 `14c21a68ecc59a671c58bf7909f597ce6d0fc862496a040d08d523c85bb126e9`. The list and its method are archiewood's work. The upstream repo has no license file, so this copy stays in this private repo only. The upstream README also names `load-bearing`, which is not in the CSV, so `banned.txt` adds it.
