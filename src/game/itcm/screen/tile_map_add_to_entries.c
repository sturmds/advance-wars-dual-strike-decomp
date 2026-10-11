#include "types.h"

/* Adds a value to every halfword of a tile map: the low bits of a map
 * entry are its tile number, so this shifts the tiles used. */
void TileMap_AddToEntries(u16 *entries, u16 byteCount, int addend)
{
    if (byteCount == 0) {
        return;
    }
    do {
        *entries++ += addend;
        byteCount -= 2;
    } while (byteCount != 0);
}
