#include "types.h"

void Gfx_RequestLayerCopy(int layer);
void func_0201e8c4(u16 *row, int x, s16 width, int palette);
void func_0201e7e4(u16 *row, int x, s16 width, u16 rowKind, int palette);
void func_0201e71c(u16 *row, int x, s16 width, int palette);

/* Writes a window frame of width x height tiles at tile (x, y) of a
 * 32-tile-wide map: a top row, height - 2 middle rows and a bottom row, each
 * with palette bank palette, then asks for layer 1 to be copied to VRAM. */
void Window_DrawFrame(int x, int y, int width, int height, u16 *map, int palette)
{
    u16 *row = map + x + y * 32;
    u16 i;
    int middleRows;

    func_0201e8c4(row, x, width, palette);
    middleRows = height - 2;
    row += 32;
    for (i = 0; i < middleRows; i++) {
        func_0201e7e4(row, x, width, 1, palette);
        row += 32;
    }
    func_0201e71c(row, x, width, palette);
    Gfx_RequestLayerCopy(1);
}
