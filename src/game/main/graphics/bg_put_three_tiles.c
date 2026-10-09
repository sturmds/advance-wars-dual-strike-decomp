#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280 *data_027c0280;

/* Writes three tiles side by side into the 32-tile-wide BG screen at
 * unk_ec: tiles firstTile to firstTile + 2 at (x, y), with palette. */
void Bg_PutThreeTiles(int x, int y, int firstTile, int palette)
{
    int pos = x + y * 32;

    ((u16 *)data_027c0280->unk_ec)[pos] = firstTile | (palette << 12);
    ((u16 *)data_027c0280->unk_ec)[pos + 1] = (firstTile + 1) | (palette << 12);
    ((u16 *)data_027c0280->unk_ec)[pos + 2] = (firstTile + 2) | (palette << 12);
}
