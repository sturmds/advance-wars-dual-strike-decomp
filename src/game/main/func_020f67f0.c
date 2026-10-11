#include "types.h"

void MapBg_DrawTileColumn(void);
void func_020f69c0(int a0, int a1, int a2, int a3);
BOOL func_020f788c(void);
void func_020f68b8(int a0, int a1, int a2, int a3);

void func_020f67f0(int a0, int a1, int a2, int a3)
{
    MapBg_DrawTileColumn();
    func_020f69c0(a0, a1, a2, a3);
    if (func_020f788c()) {
        func_020f68b8(a0, a1, a2, a3);
    }
}
