#include "types.h"
#include "game/map_data.h"

void func_020d3dec(u16 *screen, int firstTile)
{
    int x;
    int y;
    int tile;
    int width;
    int rows;

    tile = 0;
    rows = (((data_0229eadc->height + 1) & ~1) >> 1) << 4;
    width = (data_0229eadc->width + 1) & ~1;
    for (y = 0; y < rows; y += 16) {
        for (x = 0; x < width >> 1; x++) {
            screen[y * 2 + x] = firstTile + tile;
            tile++;
        }
    }
}
