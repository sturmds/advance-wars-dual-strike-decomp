#include "types.h"

void Oam_PutSizedCells(int x, int y, int width, int height, int attr2, int tile);

/* Draws a 32x16 sprite at x 112, y 0, tile 992, palette 15. */
void func_020efb64(void)
{
    Oam_PutSizedCells(112, 0, 32, 16, 0xf000, 992);
}
