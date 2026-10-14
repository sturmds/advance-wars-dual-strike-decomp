#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Clears the 32 by 32 entries of the layer 0 tile map buffer. */
void Layer0_ClearMap(void) {
  int i;
  u16* map = data_027c0280->layer0Map;

  for (i = 0; i < BG_SCREEN_ENTRIES; i++) {
    map[i] = 0;
  }
}
