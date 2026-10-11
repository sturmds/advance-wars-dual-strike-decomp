#include "types.h"

void MapBg_DrawTileRow(void);
void func_020f6a00(int bufX, int bufY, int mapX, int mapY);
BOOL func_020f788c(void);
void func_020f6948(int bufX, int bufY, int mapX, int mapY);

void MapView_DrawTopRow(int bufX, int bufY, int mapX, int mapY)
{
    MapBg_DrawTileRow();
    func_020f6a00(bufX, bufY, mapX, mapY);
    if (func_020f788c()) {
        func_020f6948(bufX, bufY, mapX, mapY);
    }
}
