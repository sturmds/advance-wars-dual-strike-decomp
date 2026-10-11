#include "types.h"

void MapBg_DrawTileRow(void);
void MapView_DrawUnitRow(int bufX, int bufY, int mapX, int mapY);
BOOL func_020f788c(void);
void MapView_DrawRangeRow(int bufX, int bufY, int mapX, int mapY);

void MapView_DrawTopRow(int bufX, int bufY, int mapX, int mapY)
{
    MapBg_DrawTileRow();
    MapView_DrawUnitRow(bufX, bufY, mapX, mapY);
    if (func_020f788c()) {
        MapView_DrawRangeRow(bufX, bufY, mapX, mapY);
    }
}
