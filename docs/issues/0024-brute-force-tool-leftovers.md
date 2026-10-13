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
- `BuildInfo` and `func_02000afc`: a file that covers `BuildInfo` makes `dsd rom config` fail. The tool avoids it (096355df) but the delinks for that area are not modelled. Since batch 43 `src/sdk/crt0/build_info.c` covers `BuildInfo` with its own `delinks.txt` entry and `dsd rom config` passes.

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

## Seen in batch 10 (family heads)

Agents matched 38 of 40 family heads; propagate.py then matched about two thirds of the siblings. Agents wrote the rest by hand or with their own scripts. The misses:

- The head writes a varying value as a struct field (sound archive table offsets), an enum name (`UNIT_STEALTH` against `UNIT_SUB`), a macro (`AI_BEHAVIOUR_DEFENSE`) or `sizeof`. The value map only sees numbers in the C, so these give `no_map`. Mapping offsets to fields and values to enum or macro names would cover them.
- `return 0`/`return 1` were rewritten by the value map; writing `TRUE`/`FALSE` in the head avoided it.
- Siblings that read the same table in another index order, or call a callee with another argument count.
- `pick --families` groups by mnemonics alone, which puts different shapes in one family (`func_02015efc`: one-argument against two-argument callee).

