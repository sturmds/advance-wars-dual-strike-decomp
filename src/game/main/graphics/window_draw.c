#include "types.h"

void func_0201e478(int x, int y, int width, int height, u16 *map, int palette);

/* Draws a window frame of width x height tiles at tile (x, y) of map; the
 * work is done by func_0201e478. */
void Window_Draw(int x, int y, int width, int height, u16 *map, int palette)
{
    func_0201e478(x, y, width, height, map, palette);
}
