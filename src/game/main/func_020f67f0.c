#include "types.h"

void MapBg_DrawTileColumn(int col, int row, int tileX, int tileY);
void MapView_DrawUnitColumn(int col, int row, int tileX, int tileY);
BOOL func_020f788c(void);
void MapView_DrawRangeColumn(int col, int row, int tileX, int tileY);

void MapView_DrawRightColumn(int col, int row, int tileX, int tileY)
{
    MapBg_DrawTileColumn(col, row, tileX, tileY);
    MapView_DrawUnitColumn(col, row, tileX, tileY);
    if (func_020f788c()) {
        MapView_DrawRangeColumn(col, row, tileX, tileY);
    }
}
