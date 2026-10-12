# 0070: `fn.py check` and `fn.py diff` count a different number of differing instructions for the same C

- **Status:** closed
- **Area:** tools
- **Opened:** 2026-10-12
- **Milestone:** M2

## Problem

Partials are ranked by the count on the last line of `fn.py diff` (issue 0067), but agents try their variants with `fn.py check`, and the two counts disagree for the same C:

| Function or variant | `check` | `diff` |
|---|---:|---:|
| func_ov004_022bacd8, `srcRow` first (batch 38) | 11 | 15 |
| a variant with `records = &gPlayRecords` | 12 | 15 |
| a func_ov003_02361c7c variant | 18 | 20 |
| Ov004_BlitBlockWithFlags (parked) | 19 | 20 |
| UnitInfoPanel_DrawPage (mainline) | 12 | 14 |
| func_ov002_02357e5c (mainline) | 21 | 14 |

`check` can be higher or lower. An agent that ranks its attempts by `check` can keep a variant that `diff` scores worse, or throw away one that `diff` scores better, and then reports a `BEST:` count from `diff` for a variant it never compared that way.

## Cause

`check` (`fnindex.compare`) compares our bytes with the original's at the same offset and counts the units (ARM words, Thumb halfwords) that differ. One missing or extra instruction makes every one after it differ. `diff` runs objdiff-cli, which lines the two listings up by opcode first: a missing or extra instruction is one row with an empty side, and a changed instruction can become two rows, one on each side. objdiff also compares relocations by symbol name, where `check` compares their target addresses (the `=` rows of issue 0017).

## Fixed (2026-10-12)

When the compare by position fails, `fnindex.check` now runs objdiff-cli on the original's delinked object from the last build (`build/delinks/`, `build/arm7/delinks/`, for the file the index gives the function) and on its own temporary object, and counts the rows with the code `fn.py diff` uses (`fn.score_rows`, `fn.count_differ`). `fn.py check` then prints diff's last line followed by the count by position:

```
UnitInfoPanel_DrawPage: 14 differ, 89.08% match (12 of 75 instructions differ by position, 44 bytes)
```

objdiff-cli on two objects takes about 6 ms, so `check` is still fast. Whether the function matches is still decided by the compare by position, which also checks relocation targets as addresses. A failed compare never counts fewer than 1 row.

`Result.differ` holds the aligned count, so `permuter.py` and `propagate.py` rank by it as well; `Result.percent` holds objdiff's percentage and `Result.by_position` the old count.

## Left as it was

- With no delinked object (a file not yet in `delinks.txt`, a function the file does not cover, or no build yet), `check` counts by position and says so in its output. `docs/workflow.md` says not to rank by that count.
- `check` reads the delinked object without running ninja. When `delinks.txt` changed since the last build, the object can lack the function, and `check` falls back to the count by position.

## Tests

`tools/tests/test_fncheck.py` fails when `check`'s count differs from the one `diff` prints for a function `check` could score with objdiff-cli; on mainline at the fix, all 19 functions in started files agree, including UnitInfoPanel_DrawPage and func_ov002_02357e5c. `tools/tests/test_ranking.py` (part of `make test`) checks `fn.count_differ` against the sample listing and an inserted instruction.
