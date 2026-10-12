#include "types.h"

void Palette_LoadWithCopy(const void* src, int offset, u32 size);

extern u8 data_02143f38[];
extern u8 data_02143f3c[];
extern u8 data_02147f40[];
extern u8 data_021480c0[];

void func_020f84b0(int flag, int slot) {
  u8* first;
  u8* second;

  if (flag != 0) {
    first = data_021480c0;
    second = data_02143f3c;
  } else {
    first = data_02147f40;
    second = data_02143f38;
  }
  Palette_LoadWithCopy(first + 26, (u16)((slot + 1) * 32 + 26), 2);
  Palette_LoadWithCopy(first + 26, (u16)((slot + 2) * 32 + 26), 2);
  Palette_LoadWithCopy(second, 126, 2);
}
