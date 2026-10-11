#include "types.h"

void Oam_PutSizedCells(int x, int y, int width, int height, int attr2, int tile);

/* Draws a 32x16 sprite at x 80, y 0, tile 1016, palette 15. */
void func_020efaf4(void)
{
    Oam_PutSizedCells(80, 0, 32, 16, 0xf000, 1016);
}
