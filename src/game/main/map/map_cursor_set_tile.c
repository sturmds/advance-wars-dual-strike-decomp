#include "types.h"

/* Writes a tile position to tile[0..1] and the same position in pixels
 * (16 per tile) to pixel[0..1]. */
void MapCursor_SetTile(int x, int y, s16 *tile, u16 *pixel)
{
    tile[0] = x;
    tile[1] = y;
    pixel[0] = x << 4;
    pixel[1] = y << 4;
}
