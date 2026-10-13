# 0078: apply_commit.py dropped local prototypes whose header is included only under #else

- **Status:** closed
- **Area:** tools
- **Opened:** 2026-10-13
- **Milestone:** M2

## Problem

`drop_header_prototypes` in `tools/apply_commit.py` removed a local prototype when any `#include` in the file, inside a preprocessor conditional or not, brought in a header that declares the function. The soft-float files include `soft_float.h` only in the `#else` (non-mwcc) branch, so the replay dropped `__fp_status`, `__fp_trap_f` and `__fp_trap_ff` and the mwcc build failed with an undefined label. `land_self.py` exited 1 three times (b41/flt). `include/sdk/fp_status.h` (f9b9f25b7) worked around it.

## Fix

Only includes outside `#if`/`#ifdef`/`#ifndef` blocks count, and only a prototype outside such blocks is dropped. `tools/tests/test_landing.py` (`LocalPrototypeTest`) covers an unconditional include, a header under `#else`, and a prototype outside the condition.
