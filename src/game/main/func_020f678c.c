#include "types.h"

void MapBg_DrawTileRow(int col, int row, int tileX, int tileY);
void func_020f6a00(int col, int row, int tileX, int tileY);
BOOL func_020f788c(void);
void func_020f6948(int col, int row, int tileX, int tileY);

void MapView_DrawTopRow(int col, int row, int tileX, int tileY)
{
    MapBg_DrawTileRow(col, row, tileX, tileY);
    func_020f6a00(col, row, tileX, tileY);
    if (func_020f788c()) {
        func_020f6948(col, row, tileX, tileY);
    }
}
