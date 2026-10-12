#include "types.h"

int strlen(void);

int Text_GetCenteredX(void) {
  int t0 = strlen();
  return (0x100 - (t0 << 3)) >> 1;
}
