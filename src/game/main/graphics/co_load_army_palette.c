#include "types.h"

extern u32 gLinkPlay;

void Co_LoadPalette(int coId, int paletteSlot);
void Co_CopyPalette(int coId, int paletteSlot,
                    int palette); /* copies one 16-colour palette of the CO */
int Co_GetArmyPaletteVariantRaw(
    int army, int coId); /* palette variant of this CO in this army's colours */

/* Loads the palette of a CO picture into palette slot paletteSlot, in the
 * colours of an army. Bit 7 of coId uses the plain palette (Co_LoadPalette);
 * gLinkPlay being 0 means army 0. */
void Co_LoadArmyPalette(int coId, int paletteSlot, int army) {
  if (coId & 0x80) {
    Co_LoadPalette(coId, paletteSlot);
    return;
  }
  if (gLinkPlay == 0) {
    army = 0;
  }
  Co_CopyPalette(coId, paletteSlot, Co_GetArmyPaletteVariantRaw(army, coId));
}
