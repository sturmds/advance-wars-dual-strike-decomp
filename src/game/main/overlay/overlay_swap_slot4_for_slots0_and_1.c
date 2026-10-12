#include "types.h"

void Overlay_Unload(int slot);
void Overlay_Load(int slot);

/* Unloads the overlay in slot 4 and loads slots 1 and 0. */
void Overlay_SwapSlot4ForSlots0And1(void) {
  Overlay_Unload(4);
  Overlay_Load(1);
  Overlay_Load(0);
}
