#include "sdk/wl.h"

/* Sorted lists of baseband registers that do not read back what was written;
   the first for RF chip type 5. */
extern const u16 gWlBbSkipRegsRf5[];
extern const u16 gWlBbSkipRegs[];

#define WIFI_BASEBAND_REGISTERS 105

/* Needed to match: this file sees the register read as returning int, so
   it narrows each result to u16 again. */
#define ReadBbAsInt(reg) (((int (*)(u32))WifiBaseband_Get)(reg))

/*
 * Writes 0xff, 0, 0x55/0xaa alternating, a count down from 0xff and each
 * single bit to every baseband register, reading each back except the ones
 * in the skip list.
 *
 * Needed to match: pos, reg, skipPos, value and pattern are shared between
 * the loops in this order, and pos serves as the register index in one loop
 * and as the skip index in the last two.
 */
void WifiSelfTest_Baseband(void) {
  u32 pos;
  u32 errors;
  u32 bit;
  u32 reg;
  u16 value;
  const u16* skip;
  u16 pattern;
  u32 skipPos;
  u16 got;

  errors = 0;
  if (WIFI_DRIVER_STATE->rf.type == 5) {
    skip = gWlBbSkipRegsRf5;
  } else {
    skip = gWlBbSkipRegs;
  }

  value = 0xff;
  for (reg = 0; reg < WIFI_BASEBAND_REGISTERS; reg++) {
    if (WifiBaseband_Put(reg, value) == -1) {
      errors = 1;
      goto done;
    }
  }
  for (reg = 0, skipPos = 0; reg < WIFI_BASEBAND_REGISTERS; reg++) {
    if (reg == skip[skipPos]) {
      skipPos++;
    } else {
      got = ReadBbAsInt(reg);
      if (got != value && errors++ > WIFI_SELFTEST_ERROR_CAP) {
        goto done;
      }
    }
  }

  value = 0;
  for (reg = 0; reg < WIFI_BASEBAND_REGISTERS; reg++) {
    WifiBaseband_Put(reg, value);
  }
  for (reg = 0, skipPos = 0; reg < WIFI_BASEBAND_REGISTERS; reg++) {
    if (reg == skip[skipPos]) {
      skipPos++;
    } else {
      got = ReadBbAsInt(reg);
      if (got != value && errors++ > WIFI_SELFTEST_ERROR_CAP) {
        goto done;
      }
    }
  }

  pattern = 0x55;
  for (reg = 0; reg < WIFI_BASEBAND_REGISTERS; reg++, pattern = ~pattern) {
    WifiBaseband_Put(reg, pattern);
  }
  value = 0x55;
  for (pos = 0, skipPos = 0; pos < WIFI_BASEBAND_REGISTERS;
       pos++, value = ~value & 0xff) {
    if (pos == skip[skipPos]) {
      skipPos++;
    } else {
      got = ReadBbAsInt(pos);
      if (got != value && errors++ > WIFI_SELFTEST_ERROR_CAP) {
        goto done;
      }
    }
  }

  pattern = 0xff;
  for (reg = 0; reg < WIFI_BASEBAND_REGISTERS; reg++, pattern--) {
    WifiBaseband_Put(reg, pattern);
  }
  pattern = 0xff;
  for (reg = 0, pos = 0; reg < WIFI_BASEBAND_REGISTERS; reg++, pattern--) {
    if (reg == skip[pos]) {
      pos++;
    } else {
      got = ReadBbAsInt(reg);
      if (got != pattern && errors++ > WIFI_SELFTEST_ERROR_CAP) {
        goto done;
      }
    }
  }

  for (reg = 0, pos = 0; reg < WIFI_BASEBAND_REGISTERS; reg++) {
    if (reg == skip[pos]) {
      pos++;
    } else {
      for (value = 1, bit = 0; bit < 9; value = (value << 1) & 0xff, bit++) {
        WifiBaseband_Put(reg, value);
        got = ReadBbAsInt(reg);
        if (got != value && errors++ > WIFI_SELFTEST_ERROR_CAP) {
          goto done;
        }
      }
    }
  }

done:
  if (errors != 0) {
    WIFI_DRIVER_STATE->state.unk_12 |= WIFI_SELFTEST_FAIL_BASEBAND;
  }
}
