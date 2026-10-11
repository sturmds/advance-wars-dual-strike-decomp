#include "types.h"

void Oam_PutSizedCells(int x, int y, int width, int height, int attr2, int tile);

/* Draws the center touch button of the map screen (MapTouch_ReadButtons):
 * a 32x16 sprite at x 112, y 0, tile 992, palette 15. */
void MapScreen_DrawCenterButton(void)
{
    Oam_PutSizedCells(112, 0, 32, 16, 0xf000, 992);
}
