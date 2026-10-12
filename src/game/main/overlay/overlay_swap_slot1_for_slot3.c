#include "types.h"

void Overlay_Unload(
    int slot); /* unloads the overlay in this slot of the overlay table, if loaded */
void Overlay_Load(int slot); /* loads the overlay in this slot, if not loaded */

void Overlay_SwapSlot1ForSlot3(void) {
  Overlay_Unload(1);
  Overlay_Load(3);
}
