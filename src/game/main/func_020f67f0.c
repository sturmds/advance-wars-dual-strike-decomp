#include "types.h"

void MapBg_DrawTileColumn(int col, int row, int tileX, int tileY);
void func_020f69c0(int col, int row, int tileX, int tileY);
BOOL func_020f788c(void);
void func_020f68b8(int col, int row, int tileX, int tileY);

void MapView_DrawRightColumn(int col, int row, int tileX, int tileY)
{
    MapBg_DrawTileColumn(col, row, tileX, tileY);
    func_020f69c0(col, row, tileX, tileY);
    if (func_020f788c()) {
        func_020f68b8(col, row, tileX, tileY);
    }
}
