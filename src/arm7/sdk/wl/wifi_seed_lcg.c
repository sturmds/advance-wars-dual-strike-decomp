#include "sdk/wl.h"

/* mul = 5 mod 8 and an odd add give the generator its full period of 65536. */
void Wifi_SeedLcg(u32 mul, u32 add) {
  WlRandom* random = &WIFI_DRIVER_STATE->random;

  random->mul = (mul & 0xfff8) + 5;
  random->add = add | 1;
}
