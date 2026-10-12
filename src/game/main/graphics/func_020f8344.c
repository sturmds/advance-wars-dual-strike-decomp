#include "types.h"
#include "game/Unk_022a7c5c.h"

void GfxQueue_AddKind2(const void* src, void* dst, u32 size);
void Palette_Load(const void* src, u32 offset, u32 size);

extern u16 data_02139308[];

void func_020f8344(int value) {
  u16* colour = &data_02139308[(value >> 2) & 15];

  GfxQueue_AddKind2(colour, data_022a7c5c->unk_44 + 8, 2);
  Palette_Load(colour, 0x228, 2);
}
