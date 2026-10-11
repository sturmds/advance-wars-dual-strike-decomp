#include "types.h"

void MapBg_DrawTileRow(void);
void func_020f6a00(int a0, int a1, int a2, int a3);
BOOL func_020f788c(void);
void func_020f6948(int a0, int a1, int a2, int a3);

void MapView_DrawBottomRow(int a0, int a1, int a2, int a3)
{
    MapBg_DrawTileRow();
    func_020f6a00(a0, a1, a2, a3);
    if (func_020f788c()) {
        func_020f6948(a0, a1, a2, a3);
    }
}
