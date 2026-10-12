#include "types.h"
#include "game/play_records.h"

/* Sets the palette variant (0-7) in the saved record of a CO; ignores ids past the last CO. */
void Co_SetSavedPaletteVariant(int co, int variant) {
  if (co >= CO_COUNT) {
    return;
  }
  gPlayRecords.coRecords[co].paletteVariant = variant;
}
