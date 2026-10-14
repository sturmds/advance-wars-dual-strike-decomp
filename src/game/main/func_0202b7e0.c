#include "types.h"

void Palette_LoadFromFile(void*, int, int, int);
extern char data_02159928[];

void func_0202b7e0(int a0) {
  Palette_LoadFromFile(data_02159928, 0, (u32)(a0 << 21) >> 16, 32);
}
