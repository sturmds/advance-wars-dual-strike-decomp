#include "types.h"
#include "game/co_graphics.h"

extern u32 gLinkPlay;

int Co_GetArmyPictureVariant(int coId, int army);
void Co_LoadArmyPalette(int coId, int paletteSlot, int army);
void TransferQueue_Add(
    const void* src, void* dst,
    u32 size); /* copies now, or queues the copy when func_01ff9eac says so */

/* Picture tile pointers at gCoGraphics + 0x40, one row per CO. */
extern const void* const data_02152b78[][sizeof(CoGraphics) / sizeof(void*)];

/* Co_QueueSmallPicture in an army's colours: the 12-tile picture of a CO goes
 * to dest through TransferQueue_Add, and its palette to paletteSlot. Bit 7 of
 * coId, or gLinkPlay being 0, means army 0. */
void Co_QueueArmySmallPicture(int coId, void* dest, int paletteSlot, int army) {
  if (gLinkPlay == 0 || (coId & 0x80)) {
    army = 0;
  }
  TransferQueue_Add(
      data_02152b78[coId & 0x7f][Co_GetArmyPictureVariant(coId, army)], dest,
      0x180);
  Co_LoadArmyPalette(coId, paletteSlot, army);
}
