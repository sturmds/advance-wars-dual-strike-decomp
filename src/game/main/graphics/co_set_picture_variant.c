#include "types.h"
#include "game/co_data.h"

void Co_SetSavedPictureVariant(int co, int variant);

/* Stores the picture variant (0 or 1) for a CO. CO_NONE and ids past the
 * last CO are ignored. */
void Co_SetPictureVariant(int coId, int variant) {
  if (coId == CO_NONE) {
    return;
  }
  if (coId >= CO_COUNT) {
    return;
  }
  Co_SetSavedPictureVariant(coId, variant);
}
