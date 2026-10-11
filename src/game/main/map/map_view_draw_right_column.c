#include "types.h"

void MapBg_DrawTileColumn(void);
void func_020f69c0(int bufX, int bufY, int mapX, int mapY);
BOOL func_020f788c(void);
void func_020f68b8(int bufX, int bufY, int mapX, int mapY);

void MapView_DrawRightColumn(int bufX, int bufY, int mapX, int mapY)
{
    MapBg_DrawTileColumn();
    func_020f69c0(bufX, bufY, mapX, mapY);
    if (func_020f788c()) {
        func_020f68b8(bufX, bufY, mapX, mapY);
    }
}
