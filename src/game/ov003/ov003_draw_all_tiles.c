#include "types.h"
#include "game/ov003_unit.h"

void func_01ff8c84(int);
void Gfx_RequestLayerCopy(int);
void func_ov003_02354444(u8 x, u8 y);

/* Draws every tile of the map with func_ov003_02354444, then requests copies of layers 1 to 3.
 * The argument is not used. */
int Ov003_DrawAllTiles(int unused)
{
    int x;
    int y;

    func_01ff8c84(1);
    for (y = 0; y < data_ov003_02364a88->height; y++) {
        for (x = 0; x < data_ov003_02364a88->width; x++) {
            func_ov003_02354444(x, y);
        }
    }
    Gfx_RequestLayerCopy(1);
    Gfx_RequestLayerCopy(2);
    Gfx_RequestLayerCopy(3);
}
