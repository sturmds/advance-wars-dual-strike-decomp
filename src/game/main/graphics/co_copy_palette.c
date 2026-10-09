#include "types.h"
#include "game/co_graphics.h"

void func_02003188(const void *src, u16 offset, u32 size);  /* copies into the current screen's palette buffer */

/* Copies 16-colour palette number palette of a CO into palette slot
 * paletteSlot of the current screen. */
void Co_CopyPalette(int coId, int paletteSlot, int palette)
{
    func_02003188(gCoGraphics[coId & 0x7f].palettes + palette * 32, paletteSlot * 32, 32);
}
