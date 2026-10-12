#include "types.h"
#include "game/play_records.h"

/* Sets the picture variant (0 or 1) in the saved record of a CO; ignores ids past the last CO. */
void Co_SetSavedPictureVariant(int co, int variant) {
  if (co >= CO_COUNT) {
    return;
  }
  gPlayRecords.coRecords[co].pictureVariant = variant;
}
