#include "types.h"
#include "game/Unk_022a7c5c.h"

void GfxQueue_AddKind2(const void* src, void* dst, u32 size);

extern u8 data_02143f38[];
extern u8 data_02143f3c[];
extern u8 data_02147f40[];
extern u8 data_021480c0[];
extern u32 data_02169d7c[];
extern u32 data_02169dbc[];

void func_020f83d8(int flag) {
  Unk_022a7c5c* gfx = data_022a7c5c;
  u8* first;
  u8* second;
  int i;

  if (flag != 0) {
    first = data_021480c0;
    second = data_02143f3c;
  } else {
    first = data_02147f40;
    second = data_02143f38;
  }
  for (i = 0; data_02169dbc[i] != 0; i++) {
    GfxQueue_AddKind2(first + 26, (u8*)gfx->texPaletteOffset + data_02169dbc[i],
                      2);
  }
  for (i = 0; data_02169d7c[i] != 0; i++) {
    GfxQueue_AddKind2(second, (u8*)gfx->texPaletteOffset + data_02169d7c[i], 2);
  }
}
