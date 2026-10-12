#include "types.h"

void Oam_PutSprite(int, int, void*, int);
extern u8 data_02166320[];

void func_020cb0f4(int a0, int a1) {
  Oam_PutSprite(a0 & 0x1ff, a1 | 0x400, data_02166320, 0);
}
