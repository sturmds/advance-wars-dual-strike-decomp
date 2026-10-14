#include "sdk/wl.h"

#define WIFI_SELFTEST_MAC_TARGETS 27

/* Test patterns 0xffff, 0x5a5a, 0xa5a5. */
extern const u16 gWlTestPatterns[];
/* The MAC registers tested, with the bits of each that read back. */
extern const WlRegMask gWlTestRegs[WIFI_SELFTEST_MAC_TARGETS];

/* Writes each pattern to every register in gWlTestRegs and reads it back,
   then a different value per register (multiples of 0x1234) so that two
   registers aliasing each other show up, then zero. */
void WifiSelfTest_MacRegisters(void) {
  volatile u16* reg;
  u32 i;
  u32 bits;
  u16 value;
  u32 errors;
  u32 pattern;

  errors = 0;

  for (pattern = 0; pattern < 3; pattern++) {
    for (i = 0; i < WIFI_SELFTEST_MAC_TARGETS; i++) {
      reg = &WIFI_MAC_REGISTER(gWlTestRegs[i].offset);
      bits = gWlTestPatterns[pattern] & gWlTestRegs[i].mask;
      *reg = bits;
      if (*reg != bits && errors++ > WIFI_SELFTEST_ERROR_CAP) {
        goto done;
      }
    }
  }

  for (i = 0, value = 0x1234; i < WIFI_SELFTEST_MAC_TARGETS; i++) {
    WIFI_MAC_REGISTER(gWlTestRegs[i].offset) = value & gWlTestRegs[i].mask;
    value += 0x1234;
  }
  for (i = 0, value = 0x1234; i < WIFI_SELFTEST_MAC_TARGETS; i++) {
    if (WIFI_MAC_REGISTER(gWlTestRegs[i].offset) !=
            (value & gWlTestRegs[i].mask) &&
        errors++ > WIFI_SELFTEST_ERROR_CAP) {
      goto done;
    }
    value += 0x1234;
  }

  for (i = 0; i < WIFI_SELFTEST_MAC_TARGETS; i++) {
    reg = &WIFI_MAC_REGISTER(gWlTestRegs[i].offset);
    *reg = 0;
    if (*reg != 0 && errors++ > WIFI_SELFTEST_ERROR_CAP) {
      goto done;
    }
  }

done:
  if (errors != 0) {
    WIFI_DRIVER_STATE->state.unk_12 |= WIFI_SELFTEST_FAIL_MAC;
  }
}
