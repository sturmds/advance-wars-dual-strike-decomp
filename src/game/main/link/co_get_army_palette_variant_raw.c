#include "types.h"
#include "game/link_sync_task.h"
#include "game/co_data.h"

int Co_GetSavedPaletteVariant(int coId);

/* Palette variant of a CO for a link player (army 1-4); army 0 reads the saved value. */
int Co_GetArmyPaletteVariantRaw(int army, int coId) {
  if (army == 0) {
    return Co_GetSavedPaletteVariant(coId & CO_ID_MASK);
  }
  return data_0229deec[army - 1][coId & CO_ID_MASK].paletteVariant;
}
