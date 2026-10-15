#include "types.h"
#include "game/overlay.h"
#include "sdk/fs.h"

u32 gOverlayLoaded[OVERLAY_SLOT_COUNT];
/* overlay id of each slot (0 to 6) */
u32 data_02151ca0[] = {
#include "data/main/data_02151ca0.inc.c"
};

/* Unloads the overlay of this slot if it is loaded. */
void Overlay_Unload(int slot) {
  if (gOverlayLoaded[slot] == 0) {
    return;
  }
  FS_UnloadOverlay(0, data_02151ca0[slot]);
  gOverlayLoaded[slot] = 0;
}
