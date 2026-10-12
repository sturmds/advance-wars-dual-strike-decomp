#include "types.h"
#include "game/play_records.h"

/* Palette variant (0-7) stored in the saved record of a CO; 0 for ids past the last CO. */
int Co_GetSavedPaletteVariant(int coId) {
  if (coId >= CO_COUNT) {
    return 0;
  }
  return gPlayRecords.coRecords[coId].paletteVariant;
}
