#include "types.h"
#include "game/overlay.h"
#include "sdk/fs.h"

extern u32 data_02151ca0[]; /* overlay id of each slot (0 to 6) */

/* Loads the overlay of this slot unless it is already loaded. */
void Overlay_Load(int slot) {
  if (gOverlayLoaded[slot] != 0) {
    return;
  }
  if (FS_LoadOverlay(0, data_02151ca0[slot])) {
    gOverlayLoaded[slot] = 1;
  }
}
