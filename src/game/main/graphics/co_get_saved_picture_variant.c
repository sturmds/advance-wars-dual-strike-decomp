#include "types.h"
#include "game/play_records.h"

/* Picture variant stored in the saved record of a CO (0 or 1). */
int Co_GetSavedPictureVariant(int coId) {
  if (coId >= CO_COUNT) {
    return 0;
  }
  return gPlayRecords.coRecords[coId].pictureVariant;
}
