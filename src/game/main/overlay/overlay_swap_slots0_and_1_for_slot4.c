#include "types.h"

void Overlay_Unload(int slot);
void Overlay_Load(int slot);

/* Unloads the overlays in slots 0 and 1 and loads slot 4. */
void Overlay_SwapSlots0And1ForSlot4(void) {
  Overlay_Unload(0);
  Overlay_Unload(1);
  Overlay_Load(4);
}
