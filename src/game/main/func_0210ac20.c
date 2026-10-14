#include "types.h"
#include "sdk/gx.h"

void func_0210ac20(int a0) {
  gGxVramBankMasks[0] |= a0;
  Vram_PowerOnBanks(a0);
}
