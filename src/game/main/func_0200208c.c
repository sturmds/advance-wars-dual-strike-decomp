#include "types.h"
#include "game/co_graphics.h"
#include "game/co_data.h"

extern u32 gLinkPlay;
int Co_GetArmyPictureVariant(int coId, int army);
void Co_LoadArmyPalette(int coId, int paletteSlot, int army);
void Mem_Copy32(const void* src, void* dst, u32 size);
extern const void* const data_02152b78[][sizeof(CoGraphics) / sizeof(void*)];

void func_0200208c(int coId, void* dest, int paletteSlot, int army) {
  if (gLinkPlay == 0 || (coId & CO_CLONE)) {
    army = 0;
  }
  Mem_Copy32(
      data_02152b78[coId & CO_ID_MASK][Co_GetArmyPictureVariant(coId, army)],
      dest, CO_SMALL_PICTURE_SIZE);
  Co_LoadArmyPalette(coId, paletteSlot, army);
}
