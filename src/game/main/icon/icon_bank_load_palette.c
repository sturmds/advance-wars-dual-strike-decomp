#include "types.h"
#include "game/icon.h"

void func_02003188(const void *src, u16 offset, u32 size);  /* copies into the current screen's palette buffer */

/* Copies the palette of icon bank bank to palette slot paletteSlot. */
void IconBank_LoadPalette(int bank, int paletteSlot)
{
    func_02003188(gIconBankInfo[bank].palette, paletteSlot * 32, 32);
}
