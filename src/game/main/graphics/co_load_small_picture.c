#include "types.h"
#include "game/co_graphics.h"
#include "game/co_data.h"

int Co_GetPictureVariant(int coId);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Co_LoadPalette(int coId, int paletteSlot);

/* Picture tile pointers at gCoGraphics + 0x40, one row per CO. */
extern const void* const data_02152b78[][sizeof(CoGraphics) / sizeof(void*)];

/* Copies the 12-tile picture of a CO (stored variant) to dest, and loads its
 * palette into paletteSlot. */
void Co_LoadSmallPicture(int coId, void* dest, int paletteSlot) {
  int variant = Co_GetPictureVariant(coId);

  Mem_Copy32(data_02152b78[coId & CO_ID_MASK][variant], dest,
             CO_SMALL_PICTURE_SIZE);
  Co_LoadPalette(coId, paletteSlot);
}
