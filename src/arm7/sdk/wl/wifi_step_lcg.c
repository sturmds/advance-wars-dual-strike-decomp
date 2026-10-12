#include "sdk/wl.h"

u16 Wifi_StepLcg(void) {
  WlRandom* random = &WIFI_DRIVER_STATE->random;

  random->value = random->value * random->mul + random->add;
  return random->value;
}
