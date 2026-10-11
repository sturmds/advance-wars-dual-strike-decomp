#include "types.h"

void MapBg_DrawTileColumn(void);
void MapView_DrawUnitColumn(int bufX, int bufY, int mapX, int mapY);
BOOL func_020f788c(void);
void MapView_DrawRangeColumn(int bufX, int bufY, int mapX, int mapY);

void MapView_DrawRightColumn(int bufX, int bufY, int mapX, int mapY)
{
    MapBg_DrawTileColumn();
    MapView_DrawUnitColumn(bufX, bufY, mapX, mapY);
    if (func_020f788c()) {
        MapView_DrawRangeColumn(bufX, bufY, mapX, mapY);
    }
}
