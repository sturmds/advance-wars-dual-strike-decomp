#include "types.h"

void Gx_DrawTexturedQuadFormat(int x, int y, int z, int sizeS, int sizeT,
                               int texFmt, u32 texAddr, u32 palAddr, int flipX,
                               int flipY);

/* Draws the 192 x 144 picture at texture offset 0x16b00 (16 colours,
 * palette offset 0x240) centred at the origin, as four quads: 128 and 64
 * pixels wide, 128 and 16 pixels high. */
void MainMenu_DrawPicture192x144(void) {
  Gx_DrawTexturedQuadFormat(-96, -72, 0, 4, 4, 3, 0x16b00, 0x240, 0, 0);
  Gx_DrawTexturedQuadFormat(32, -72, 0, 3, 4, 3, 0x18b00, 0x240, 0, 0);
  Gx_DrawTexturedQuadFormat(-96, 56, 0, 4, 1, 3, 0x19b00, 0x240, 0, 0);
  Gx_DrawTexturedQuadFormat(32, 56, 0, 3, 1, 3, 0x19f00, 0x240, 0, 0);
}
