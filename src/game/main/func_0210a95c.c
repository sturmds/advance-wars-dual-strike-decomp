#include "types.h"

void Vram_ReturnBanksToLcdc(void*, int, int);
extern u8 data_022a9886[];

void func_0210a95c(void) {
  int t0 = *((int*)0x4000000) & -0x40000001;
  *((int*)0x4000000) = t0;
  Vram_ReturnBanksToLcdc(data_022a9886, t0, 0x4000000);
}
