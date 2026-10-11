#include "types.h"

void func_020f79a8(int x, int y);

/* Draws the units of one column of 13 tiles at mapX, starting at row mapY. */
void MapView_DrawUnitColumn(int bufX, int bufY, int mapX, int mapY)
{
    u16 i;

    for (i = 0; i < 13; i++) {
        func_020f79a8(mapX, (s16)(mapY + i));
    }
}
