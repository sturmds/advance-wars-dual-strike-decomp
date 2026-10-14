#include "types.h"
#include "game/link_sync_task.h"
#include "game/co_data.h"

/* Stores the palette variant a link player (army 1-4) chose for a CO. Army 0 is ignored. */
void Co_SetArmyPaletteVariant(int army, int coId, int variant) {
  if (army == 0) {
    return;
  }
  data_0229deec[army - 1][coId & CO_ID_MASK].paletteVariant = variant;
}
