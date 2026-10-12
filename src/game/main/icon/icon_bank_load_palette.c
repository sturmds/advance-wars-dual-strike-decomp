#include "types.h"
#include "game/icon.h"

void Palette_Load(
    const void* src, u16 offset,
    u32 size); /* copies into the current screen's palette buffer */

/* Copies the palette of icon bank bank to palette slot paletteSlot. */
void IconBank_LoadPalette(int bank, int paletteSlot) {
  Palette_Load(gIconBankInfo[bank].palette, paletteSlot * 32, 32);
}
