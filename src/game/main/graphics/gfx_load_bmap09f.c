#include "types.h"

u32 Gfx_UnpackBmap09f(void* dst); /* unpacks file /bmap/09f to dst */
void Gfx_LoadBmap0a0Palette(
    int unused,
    int paletteSlot); /* loads /bmap/0a0 into a 16-colour palette slot */

/* Loads the tiles of /bmap/09f (window frame pieces and a few letters)
 * to dst and their palette into paletteSlot. */
void Gfx_LoadBmap09f(void* dst, int unused, int paletteSlot) {
  Gfx_UnpackBmap09f(dst);
  Gfx_LoadBmap0a0Palette(unused, paletteSlot);
}
