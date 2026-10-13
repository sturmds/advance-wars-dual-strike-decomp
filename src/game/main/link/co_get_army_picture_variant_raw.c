#include "types.h"
#include "game/link_sync_task.h"

int Co_GetSavedPictureVariant(int coId);

/* Picture variant of a CO for a link player (army 1-4); army 0 reads the saved value. */
int Co_GetArmyPictureVariantRaw(int army, int coId) {
  if (army == 0) {
    return Co_GetSavedPictureVariant(coId & 0x7f);
  }
  return data_0229deec[army - 1][coId & 0x7f].pictureVariant;
}
