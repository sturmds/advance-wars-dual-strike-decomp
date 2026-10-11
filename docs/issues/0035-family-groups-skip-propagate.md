# 0035: Family groups do not run `propagate.py` on their twins

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-10
- **Milestone:** M1

## Problem

The four batch 12 family groups (`fam0`-`fam3`) matched their heads but none ran `tools/propagate.py` afterwards, so the twins stayed unmatched until the orchestrator ran it between groups (31 twins, 39k tokens, `bdce6c458`) and the wrap-up ran it again (7 more).

## Workaround

The orchestrator and the wrap-up run `propagate.py` on `mainline`.

## Ideas

- Add a last step to the family prompt: run `propagate.py --only` the head's twins, commit, then land.
