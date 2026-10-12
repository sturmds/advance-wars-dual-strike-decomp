#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/ov003_unit.h"

extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern Ov003Unit* data_ov003_02369534;

void Screen_SetCurrentBoth(void* state);

#define STATE data_ov003_02364928
#define MAP data_ov003_02364a88
#define GFX data_027c0280
#define SELECTED data_ov003_02369534

/* Centres the map scroll on the selected unit (data_ov003_02369534), or when it
 * has hp 0 and bit 7 of unk_17 clear, on the tile
 * in unk_38 and unk_39 of its row, unless the current slot is in state 3. The
 * scroll is kept inside the map. The low 4 bits of it go to the scroll of BG layers 3, 2
 * and 1. Units use 128 per tile, the scroll 16 per tile, so one is shifted right by 3. */
void Ov003_ScrollToSelectedUnit(void) {
  u8 layer3;
  u8 layer2;
  u8 layer1;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  if (STATE->slots[STATE->currentSlot].state != 3) {
    if (SELECTED->hp != 0 || SELECTED->unk_17_7) {
      STATE->scrollX = (SELECTED->x >> 3) - 128;
      STATE->scrollY = (SELECTED->y >> 3) - 96;
    } else {
      STATE->scrollX =
          (STATE->rows[SELECTED->row - 1].unk_38 * 128 + 64 >> 3) - 128;
      STATE->scrollY =
          (STATE->rows[SELECTED->row - 1].unk_39 * 128 + 64 >> 3) - 96;
    }
  }
  if (STATE->scrollX < 0) {
    STATE->scrollX = 0;
  }
  if (STATE->scrollY < 0) {
    STATE->scrollY = 0;
  }
  if (STATE->scrollX > MAP->width * 16 - 257) {
    STATE->scrollX = MAP->width * 16 - 256;
  }
  if (STATE->scrollY > MAP->height * 16 - 193) {
    STATE->scrollY = MAP->height * 16 - 192;
  }
  layer2 = GFX->layer2Bg;
  layer1 = GFX->layer1Bg;
  layer3 = GFX->layer3Bg;
  GFX->bgScrollX[layer3] = STATE->scrollX & 15;
  GFX->bgScrollX[layer2] = GFX->bgScrollX[layer3];
  GFX->bgScrollX[layer1] = GFX->bgScrollX[layer2];
  layer3 = GFX->layer3Bg;
  layer2 = GFX->layer2Bg;
  layer1 = GFX->layer1Bg;
  GFX->bgScrollY[layer3] = STATE->scrollY & 15;
  GFX->bgScrollY[layer2] = GFX->bgScrollY[layer3];
  GFX->bgScrollY[layer1] = GFX->bgScrollY[layer2];
}
