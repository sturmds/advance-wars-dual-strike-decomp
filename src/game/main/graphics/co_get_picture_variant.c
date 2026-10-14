#include "types.h"
#include "game/co_data.h"

int Co_GetSavedPictureVariant(
    int coId); /* bit 30 of the CO's saved record: alternate picture */

/* Picture variant to show for a CO: the stored one for a playable CO,
 * 0 when coId is a clone (CO_CLONE) or the id is out of range. */
int Co_GetPictureVariant(int coId) {
  if (coId & CO_CLONE) {
    return 0;
  }
  coId &= 0x7f;
  if (coId >= CO_COUNT) {
    return 0;
  }
  return Co_GetSavedPictureVariant(coId);
}
