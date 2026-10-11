#include "types.h"
#include "game/ov003_unit.h"

void Gfx_ClearLayerMap(int);
void Gfx_RequestLayerCopy(int);
void Ov003_DrawTile(u8 x, u8 y);

/* Draws every tile of the map with Ov003_DrawTile, then requests copies of layers 1 to 3.
 * The argument is not used. */
int Ov003_DrawAllTiles(int unused)
{
    int x;
    int y;

    Gfx_ClearLayerMap(1);
    for (y = 0; y < data_ov003_02364a88->height; y++) {
        for (x = 0; x < data_ov003_02364a88->width; x++) {
            Ov003_DrawTile(x, y);
        }
    }
    Gfx_RequestLayerCopy(1);
    Gfx_RequestLayerCopy(2);
    Gfx_RequestLayerCopy(3);
}
