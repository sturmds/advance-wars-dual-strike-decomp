#include "types.h"
#include "game/oam_cells.h"

void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);

/* Draws value in decimal as sprites, 7 pixels per digit. x is the right-most
 * digit; digits are drawn right to left. */
void Oam_PutNumber(int x, int y, int value, int tileAndPalette) {
  int row = y & 0xff;

  while (1) {
    Oam_PutSprite(x & 0x1ff, row, &data_027c0050, tileAndPalette + value % 10);
    value /= 10;
    if (value == 0) {
      break;
    }
    x -= 7;
  }
}
