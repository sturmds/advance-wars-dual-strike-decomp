#include "types.h"
#include "game/script_slot.h"
#include "game/Unk_02088d04.h"

extern u8 data_02140518[];

#define WIPE_TILE_DATA_OFFSET \
  0x5100 /* tile 0x288 of the layer's character base */

void Data_Unpack(const void* src, void* dest);
void ScreenWipe_SetColumn(int column, int phase, int mirror);

/* Start of the wipe that ScreenWipe_PhaseUpStep steps: scrolls the first
 * layer to 0, unpacks the wipe tiles into its character base, sets every
 * column to phase 0 mirrored and the frame to 0. */
void ScreenWipe_PhaseUpInit(WipeSlot* slot) {
  u8 column;

  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
  Data_Unpack(
      data_02140518,
      data_027c0280->bgVram +
          data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase * 0x4000 +
          WIPE_TILE_DATA_OFFSET);
  column = 0;
  do {
    ScreenWipe_SetColumn(column, 0, 1);
    column++;
  } while (column < 16);
  slot->frame = 0;
}
