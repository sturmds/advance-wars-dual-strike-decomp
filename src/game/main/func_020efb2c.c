#include "types.h"

void Oam_PutSizedCells(int x, int y, int width, int height, int attr2, int tile);

/* Draws a 32x16 sprite at x 144, y 0, tile 1008, palette 15. */
void func_020efb2c(void)
{
    Oam_PutSizedCells(144, 0, 32, 16, 0xf000, 1008);
}
