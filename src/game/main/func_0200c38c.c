#include "types.h"

void func_0200c38c(s32* out, int x, int y, int z) {
  out[0] = (x << 13) / 256;
  out[1] = -((y * 0x6000) / 768);
  out[2] = z;
}
