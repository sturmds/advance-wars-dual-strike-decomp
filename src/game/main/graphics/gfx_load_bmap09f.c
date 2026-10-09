#include "types.h"

u32 func_0201e338(void *dst);                       /* unpacks file /bmap/09f to dst */
void func_0201e2d4(int unused, int paletteSlot);    /* loads /bmap/0a0 into a 16-colour palette slot */

/* Loads the tiles of /bmap/09f (window frame pieces and a few letters)
 * to dst and their palette into paletteSlot. */
void Gfx_LoadBmap09f(void *dst, int unused, int paletteSlot)
{
    func_0201e338(dst);
    func_0201e2d4(unused, paletteSlot);
}
