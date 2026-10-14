#include "types.h"
#include "game/co_data.h"

void Co_CopyPalette(int coId, int paletteSlot,
                    int palette); /* copies one 16-colour palette of the CO */
int Co_GetSavedPaletteVariant(
    int coId); /* palette variant stored for this CO (0-7) */

/* Loads the palette of a CO picture into palette slot paletteSlot.
 * A clone (CO_CLONE) uses palette CO_CLONE_PALETTE instead of the stored
 * variant. */
void Co_LoadPalette(int coId, int paletteSlot) {
  if (coId & CO_CLONE) {
    Co_CopyPalette(coId, paletteSlot, CO_CLONE_PALETTE);
    return;
  }
  Co_CopyPalette(coId, paletteSlot,
                 Co_GetSavedPaletteVariant(coId & CO_ID_MASK));
}
