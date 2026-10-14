#include "types.h"
#include "game/Unk_02088d04.h"

void Palette_FillSlotWhite(int slot);

/* Sets all 32 palette slots of the palette buffer to white. */
void Palette_FillAllWhite(void) {
  int slot;

  for (slot = 0; slot < PALETTE_BUFFER_SIZE / PALETTE_16_SIZE; slot++) {
    Palette_FillSlotWhite(slot);
  }
}
