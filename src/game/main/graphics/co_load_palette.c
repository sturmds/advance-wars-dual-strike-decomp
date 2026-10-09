#include "types.h"

void func_02002c48(int coId, int paletteSlot, int palette);   /* copies one 16-colour palette of the CO */
int func_02017bcc(int coId);                                /* palette variant stored for this CO (0-7) */

/* Loads the palette of a CO picture into palette slot paletteSlot.
 * With bit 7 of coId set, palette 7 is used instead of the stored variant. */
void Co_LoadPalette(int coId, int paletteSlot)
{
    if (coId & 0x80) {
        func_02002c48(coId, paletteSlot, 7);
        return;
    }
    func_02002c48(coId, paletteSlot, func_02017bcc(coId & 0x7f));
}
