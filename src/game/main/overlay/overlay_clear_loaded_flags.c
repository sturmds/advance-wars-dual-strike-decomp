#include "types.h"
#include "game/overlay.h"

/* Marks every overlay slot as not loaded. */
void Overlay_ClearLoadedFlags(void) {
  int slot;

  for (slot = 0; slot < 7; slot++) {
    gOverlayLoaded[slot] = 0;
  }
}
