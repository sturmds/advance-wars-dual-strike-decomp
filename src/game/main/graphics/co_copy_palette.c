#include "types.h"
#include "game/co_graphics.h"
#include "game/co_data.h"

void Palette_Load(
    const void* src, u16 offset,
    u32 size); /* copies into the current screen's palette buffer */

/* Copies 16-colour palette number palette of a CO into palette slot
 * paletteSlot of the current screen. */
void Co_CopyPalette(int coId, int paletteSlot, int palette) {
  Palette_Load(gCoGraphics[coId & CO_ID_MASK].palettes + palette * 32,
               paletteSlot * 32, 32);
}
