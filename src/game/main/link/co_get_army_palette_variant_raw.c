#include "types.h"
#include "game/link_sync_task.h"

int Co_GetSavedPaletteVariant(int coId);

/* Palette variant of a CO for a link player (army 1-4); army 0 reads the saved value. */
int Co_GetArmyPaletteVariantRaw(int army, int coId) {
  if (army == 0) {
    return Co_GetSavedPaletteVariant(coId & 0x7f);
  }
  return data_0229deec[army - 1][coId & 0x7f].paletteVariant;
}
