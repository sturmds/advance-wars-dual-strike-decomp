#include "types.h"

/* Sets a width x height block of a 32-entry-wide tile map to one entry. */
void Bg_FillRect(u16 *map, int x, int y, int width, int height, u16 tile)
{
    u16 *entry;
    int rowIndex;
    int col;
    u16 *row;

    map += x;
    row = map + y * 32;
    for (rowIndex = 0; rowIndex < height; rowIndex++) {
        entry = row;
        for (col = 0; col < width; col++) {
            *entry++ = tile;
        }
        row += 32;
    }
}
