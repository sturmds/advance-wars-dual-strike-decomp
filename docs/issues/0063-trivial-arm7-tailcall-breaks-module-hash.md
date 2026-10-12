# 0063: `trivial.py --arm7` output for `func_arm7_03807408` passes the fast check but breaks the arm7 module hash

- **Status:** closed
- **Area:** tools
- **Opened:** 2026-10-11
- **Milestone:** M2

## Problem

`python3 tools/trivial.py --arm7` generated this for `func_arm7_03807408` (a store to 0x04000208, then a tail call):

```c
void func_arm7_03807408(void)
{
    *((u16 *)0x4000208) = 0;
    func_arm7_03807394(0x4000208, 0);
}
```

It passes the fast check and `fn.py diff`, but with the file linked, `build/build/arm7.bin` differs from `extract/arm7/arm7.bin` (`make release` stops at `check-modules`). The other 108 functions the run matched link cleanly. The function is the last one in the ARM7 WRAM `.text`, so the cause may be the end-of-section alignment or the literal pool placement.

## Done when

- Find why the linked bytes differ, then either fix the delink range or the C, or make `trivial.py` skip it.

## Cause and fix (2026-10-12)

b32-rest found the cause: `fn.py add`, which `trivial.py` calls, ended the range at the next delinked file (0x03807514), so it took in the data at 0x03807424-0x03807514 that the C object does not hold. With `end:0x03807424` the ARM7 module hash and the ROM SHA-1 match; the function is in as `Reset_MasterOffAndReboot_Arm7`. `fn.py add` now stops at the next symbol of any kind (see 0051), which gives 0x03807424 here.

Tests: `make test` (`tools/tests/test_fnadd.py`).
