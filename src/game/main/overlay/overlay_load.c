#include "types.h"

extern u32 gOverlayLoaded[7]; /* per slot: 1 while its overlay is loaded */
extern u32 data_02151ca0[7];  /* overlay id of each slot (0 to 6) */

int FS_LoadOverlay(int target,
                   u32 overlayId); /* loads an overlay; 0 on failure */

/* Loads the overlay of this slot unless it is already loaded. */
void Overlay_Load(int slot) {
  if (gOverlayLoaded[slot] != 0) {
    return;
  }
  if (FS_LoadOverlay(0, data_02151ca0[slot])) {
    gOverlayLoaded[slot] = 1;
  }
}
