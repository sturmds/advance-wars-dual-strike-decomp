#include "types.h"

void Palette_FillSlotWhite(int slot);

/* Sets all 32 palette slots of the palette buffer to white. */
void Palette_FillAllWhite(void) {
  int slot;

  for (slot = 0; slot < 32; slot++) {
    Palette_FillSlotWhite(slot);
  }
}
