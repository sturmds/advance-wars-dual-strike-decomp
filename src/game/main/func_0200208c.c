#include "types.h"
#include "game/co_graphics.h"

extern u32 gLinkPlay;
int Co_GetArmyPictureVariant(int coId, int army);
void Co_LoadArmyPalette(int coId, int paletteSlot, int army);
void Mem_Copy32(const void* src, void* dst, u32 size);
extern const void* const data_02152b78[][sizeof(CoGraphics) / sizeof(void*)];

void func_0200208c(int coId, void* dest, int paletteSlot, int army) {
  if (gLinkPlay == 0 || (coId & 0x80)) {
    army = 0;
  }
  Mem_Copy32(data_02152b78[coId & 0x7f][Co_GetArmyPictureVariant(coId, army)],
             dest, 0x180);
  Co_LoadArmyPalette(coId, paletteSlot, army);
}
