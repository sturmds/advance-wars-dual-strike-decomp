#include "types.h"

/* per slot: 1 while its overlay is loaded; also declared in src/game/main/overlay/overlay_clear_loaded_flags.c and 2 more */
u32 gOverlayLoaded[7];
extern u32 data_02151ca0[7]; /* overlay id of each slot (0 to 6) */

int FS_UnloadOverlay(int target, u32 overlayId); /* unloads an overlay */

/* Unloads the overlay of this slot if it is loaded. */
void Overlay_Unload(int slot) {
  if (gOverlayLoaded[slot] == 0) {
    return;
  }
  FS_UnloadOverlay(0, data_02151ca0[slot]);
  gOverlayLoaded[slot] = 0;
}
