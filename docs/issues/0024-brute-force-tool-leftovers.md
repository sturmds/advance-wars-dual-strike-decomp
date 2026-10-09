# 0024: What propagate.py, trivial.py and permute.py leave unmatched

- **Status:** open
- **Area:** tools
- **Opened:** 2026-10-09
- **Milestone:** M1

## Problem

The three tools matched 1,263 functions without a model. These cases still fall through.

## propagate.py

- A struct field at a different offset in the twin: the copy compiles but the field name is wrong for the twin.
- Register or condition differences between near twins.
- `static` helpers the template calls.
- Different operand counts (different instruction sequence at the same size).
- No symbol for the twin base `func_020dfcc8` in the symbol map, so its family has no mapping.
- `BuildInfo` and `func_02000afc`: a file that covers `BuildInfo` makes `dsd rom config` fail. The tool avoids it (096355df) but the delinks for that area are not modelled.

## trivial.py

- Loops and branches are not lifted.
- Element size of indexed loads and stores.
- Flag-setting arithmetic and `mla`.
- Other instruction forms (the `other` pattern matched 11 of 12).
- `ldm`, `stm` and `svc`.
- Dead frame code (the compiler removes it) leaves `func_ov005_0235ac3c`, `func_ov005_0235ac48` and `func_ov005_0235ad24` as empty functions that our C cannot reproduce.
- Wrappers, modify and store patterns still differ for a few functions (3 wrappers, 4 modifies).

## permute.py

- It scores with its own compare, not `fnindex.check`, so its score and `fn.py check` can disagree on relocations.

## Done when

Each item is fixed or split into its own issue.
