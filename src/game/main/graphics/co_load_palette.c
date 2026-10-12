#include "types.h"

void Co_CopyPalette(int coId, int paletteSlot,
                    int palette); /* copies one 16-colour palette of the CO */
int Co_GetSavedPaletteVariant(
    int coId); /* palette variant stored for this CO (0-7) */

/* Loads the palette of a CO picture into palette slot paletteSlot.
 * With bit 7 of coId set, palette 7 is used instead of the stored variant. */
void Co_LoadPalette(int coId, int paletteSlot) {
  if (coId & 0x80) {
    Co_CopyPalette(coId, paletteSlot, 7);
    return;
  }
  Co_CopyPalette(coId, paletteSlot, Co_GetSavedPaletteVariant(coId & 0x7f));
}
