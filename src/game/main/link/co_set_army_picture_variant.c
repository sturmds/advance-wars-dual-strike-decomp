#include "types.h"
#include "game/link_sync_task.h"

/* Stores the picture variant a link player (army 1-4) chose for a CO. Army 0 is ignored. */
void Co_SetArmyPictureVariant(int army, int coId, int variant) {
  if (army == 0) {
    return;
  }
  data_0229deec[army - 1][coId & 0x7f].pictureVariant = variant;
}
