#include "types.h"
#include "game/co_graphics.h"

int Co_GetPictureVariant(int coId);
void TransferQueue_Add(
    const void* src, void* dst,
    u32 size); /* copies now, or queues the copy when func_01ff9eac says so */
void Co_LoadPalette(int coId, int paletteSlot);

/* Picture tile pointers at gCoGraphics + 0x40, one row per CO. */
extern const void* const data_02152b78[][sizeof(CoGraphics) / sizeof(void*)];

/* Co_LoadSmallPicture through TransferQueue_Add: the 12-tile picture of a CO
 * (stored variant) goes to dest, maybe on a later copy pass, and its palette
 * to paletteSlot. */
void Co_QueueSmallPicture(int coId, void* dest, int paletteSlot) {
  int variant = Co_GetPictureVariant(coId);

  TransferQueue_Add(data_02152b78[coId & 0x7f][variant], dest, 0x180);
  Co_LoadPalette(coId, paletteSlot);
}
