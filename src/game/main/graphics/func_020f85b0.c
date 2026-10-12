#include "types.h"

void Palette_LoadWithCopy(const void* src, int offset, u32 size);

extern u16 data_02139450[];

void func_020f85b0(int slot, int index) {
  Palette_LoadWithCopy(&data_02139450[index], (u16)((slot + 11) * 32 + 30), 2);
}
