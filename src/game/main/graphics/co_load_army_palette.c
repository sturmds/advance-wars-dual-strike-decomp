#include "types.h"

extern u32 data_0229eaec;

void Co_LoadPalette(int coId, int paletteSlot);
void Co_CopyPalette(int coId, int paletteSlot, int palette);   /* copies one 16-colour palette of the CO */
int func_02081b4c(int army, int coId);                          /* palette variant of this CO in this army's colours */

/* Loads the palette of a CO picture into palette slot paletteSlot, in the
 * colours of an army. Bit 7 of coId uses the plain palette (Co_LoadPalette);
 * data_0229eaec being 0 means army 0. */
void Co_LoadArmyPalette(int coId, int paletteSlot, int army)
{
    if (coId & 0x80) {
        Co_LoadPalette(coId, paletteSlot);
        return;
    }
    if (data_0229eaec == 0) {
        army = 0;
    }
    Co_CopyPalette(coId, paletteSlot, func_02081b4c(army, coId));
}
