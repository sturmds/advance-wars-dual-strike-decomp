#include "types.h"

void Palette_Load(const void* src, u32 offset, u32 size);

extern u16 data_02138c88[];
extern u16 data_02139308[];

void func_020f8394(int value) {
  int index = (value >> 2) & 15;

  Palette_Load(&data_02138c88[index], 0x238, 2);
  Palette_Load(&data_02139308[index], 0x228, 2);
}
