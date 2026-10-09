#include "types.h"

void Gfx_RequestLayerCopy(int layer);
void func_0201e8c4(u16 *row, int x, s16 width, int palette);
void func_0201e7e4(u16 *row, int x, s16 width, s16 rowKind, int palette);
void func_0201e71c(u16 *row, int x, s16 width, int palette);

/* Like Window_DrawFrame, but the middle rows alternate between row kinds 1
 * and 0 (func_0201e7e4 picks the tiles by kind). */
void Window_DrawFrameStriped(int x, int y, int width, int height, u16 *map, int palette)
{
    u16 *row = map + x + y * 32;
    u16 i;
    u16 rowKind;
    int middleRows;

    func_0201e8c4(row, x, width, palette);
    middleRows = height - 2;
    row += 32;
    rowKind = 1;
    for (i = 0; i < middleRows; i++) {
        func_0201e7e4(row, x, width, rowKind, palette);
        row += 32;
        rowKind = 1 - rowKind;
    }
    func_0201e71c(row, x, width, palette);
    Gfx_RequestLayerCopy(1);
}
