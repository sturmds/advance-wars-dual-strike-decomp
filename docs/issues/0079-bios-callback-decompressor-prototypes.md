# 0079: The callback BIOS decompressors are declared with two arguments, not four

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-13
- **Milestone:** M2

## Problem

`include/sdk/svc.h` declares `Bios_UnpackLz77ByHalfwords`, `Bios_UnpackHuffman` and `Bios_UnpackRunLengthByHalfwords`, and their `_Arm7` copies, as `(const void* src, void* dst)`. On the DS these BIOS calls (0x12, 0x13, 0x15) read their input through a callback table passed in r3, with a user argument in r2 (GBATEK), so they take four arguments.

Nothing in the ROM calls the six stubs (no relocation points at them), so no caller shows the argument types, and the portable C beside them is `#error` (b41 svc, svc7).

## Fix

Add the two parameters (a user argument and a pointer to the callback table) to the six prototypes and the `asm` definitions; the bytes do not change. Then the portable C can read the source through the callbacks.
