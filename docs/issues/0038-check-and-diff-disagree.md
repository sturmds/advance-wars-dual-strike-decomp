# 0038: `fn.py check` and `fn.py diff` disagree on BattleFighterTask_Setup

- **Status:** open
- **Area:** tools
- **Opened:** 2026-10-11
- **Milestone:** M1

## Problem

While landing the partials13 job (2026-10-10 20:27), `fn.py check` reported `BattleFighterTask_Setup` 0x10 bytes short, while `fn.py diff` showed no difference. The ROM still matched, so the function links correctly. The two commands compare the function in different ways, and one of them hides a size mismatch.

## Workaround

Trust `make check` (module hashes and the whole-ROM SHA-1). Do not use `fn.py check` alone to decide that a function matches.

## Ideas

- Compare the byte length of the compiled function with the original in `fn.py diff`, and say so when only the length differs.
- Find why `fn.py check` sees 0x10 fewer bytes (a literal pool or a trailing alignment pad counted by one tool and not the other).
