#include "sdk/wl.h"

/* Writes three patterns over all of Wi-Fi RAM and reads each back: a count
   down from 0xffff, then 0x5a5a and 0xa5a5 alternating with their inverse. */
void WifiSelfTest_Ram(void) {
  u32 errors;
  u32 offset;
  volatile u16* ram;
  u16 value;

  errors = 0;

  ram = WIFI_PACKET_RAM;
  value = 0xffff;
  for (offset = 0; offset < WIFI_PACKET_RAM_BYTES; offset += 2) {
    *ram++ = value--;
  }
  ram = WIFI_PACKET_RAM;
  value = 0xffff;
  for (offset = 0; offset < WIFI_PACKET_RAM_BYTES;
       offset += 2, ram++, value--) {
    if (*ram != value && errors++ > WIFI_SELFTEST_ERROR_CAP) {
      goto done;
    }
  }

  ram = WIFI_PACKET_RAM;
  value = 0x5a5a;
  for (offset = 0; offset < WIFI_PACKET_RAM_BYTES;
       offset += 2, value = ~value) {
    *ram++ = value;
  }
  ram = WIFI_PACKET_RAM;
  value = 0x5a5a;
  for (offset = 0; offset < WIFI_PACKET_RAM_BYTES;
       offset += 2, ram++, value = ~value) {
    if (*ram != value && errors++ > WIFI_SELFTEST_ERROR_CAP) {
      goto done;
    }
  }

  ram = WIFI_PACKET_RAM;
  value = 0xa5a5;
  for (offset = 0; offset < WIFI_PACKET_RAM_BYTES;
       offset += 2, value = ~value) {
    *ram++ = value;
  }
  ram = WIFI_PACKET_RAM;
  value = 0xa5a5;
  for (offset = 0; offset < WIFI_PACKET_RAM_BYTES;
       offset += 2, ram++, value = ~value) {
    if (*ram != value && errors++ > WIFI_SELFTEST_ERROR_CAP) {
      goto done;
    }
  }

done:
  if (errors != 0) {
    WIFI_DRIVER_STATE->state.unk_12 |= WIFI_SELFTEST_FAIL_RAM;
  }
}
