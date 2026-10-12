#include "types.h"

int Palette_GetBlock(int, int);
int Mem_Copy32(int, int, int);

int Palette_CopyBlock(int a0, int a1) {
  int t0 = Palette_GetBlock(a0, 1);
  return Mem_Copy32(t0, a1, 0x100);
}
