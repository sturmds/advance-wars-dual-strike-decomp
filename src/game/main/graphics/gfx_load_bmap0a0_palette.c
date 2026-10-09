#include "types.h"

void FS_ChangeDir(const char *dir);
void func_020030cc(const char *path, int filePos, u16 paletteOffset, int size);

extern char data_021587d0[];    /* "/bmap" */
extern char data_021587d8[];    /* "0a0" */

/* Loads the 32-byte palette /bmap/0a0 into 16-colour palette slot paletteSlot.
 * The first argument is not used. */
void Gfx_LoadBmap0a0Palette(int unused, int paletteSlot)
{
    FS_ChangeDir(data_021587d0);
    func_020030cc(data_021587d8, 0, paletteSlot << 5, 32);
}
