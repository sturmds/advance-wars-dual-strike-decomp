#include "types.h"
#include "game/icon.h"

void func_02003188(const void *src, u16 offset, u32 size);  /* copies into the current screen's palette buffer */

/* Copies the palette of icon iconId's bank to palette slot paletteSlot. */
void Icon_LoadPalette(int iconId, int paletteSlot)
{
    func_02003188(gIconBankInfo[Icon_GetBank(iconId)].palette, paletteSlot * 32, 32);
}
