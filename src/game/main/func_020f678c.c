#include "types.h"

void MapBg_DrawTileRow(int col, int row, int tileX, int tileY);
void MapView_DrawUnitRow(int col, int row, int tileX, int tileY);
BOOL func_020f788c(void);
void MapView_DrawRangeRow(int col, int row, int tileX, int tileY);

void MapView_DrawTopRow(int col, int row, int tileX, int tileY)
{
    MapBg_DrawTileRow(col, row, tileX, tileY);
    MapView_DrawUnitRow(col, row, tileX, tileY);
    if (func_020f788c()) {
        MapView_DrawRangeRow(col, row, tileX, tileY);
    }
}
