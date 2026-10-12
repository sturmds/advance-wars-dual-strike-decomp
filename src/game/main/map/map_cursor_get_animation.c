#include "types.h"

BOOL Map_IsFreeDeployCity(int tileX, int tileY);
int Screen_AddCornerIndex(int x, int y, int base);

/* Returns the animation for map cursor style `style` at map pixel position
 * (*xp, *yp) and moves the position to where the sprite is drawn: most
 * styles to the tile centre (+8, +8). Styles 0, 1, 5 and 6 pick one of four
 * animations by screen corner (Screen_AddCornerIndex); style 0 uses the
 * second set over an empty tile that Map_IsFreeDeployCity accepts (a city a unit
 * can be deployed from). Other styles not listed are their own animation. */
int MapCursor_GetAnimation(int* xp, int* yp, int style) {
  int x = *xp;
  int y = *yp;

  switch (style) {
    case 0:
      x += 8;
      y += 8;
      style = Screen_AddCornerIndex(
          x, y, Map_IsFreeDeployCity((s16)(x >> 4), (s16)(y >> 4)) ? 4 : 0);
      break;
    case 1:
      x += 8;
      y += 8;
      style = Screen_AddCornerIndex(x, y, 0);
      break;
    case 2:
      x += 8;
      y += 8;
      style = 16;
      break;
    case 3:
      style = 17;
      break;
    case 5:
      x += 8;
      y += 8;
      style = Screen_AddCornerIndex(x, y, 8);
      break;
    case 6:
      x += 8;
      y += 8;
      style = Screen_AddCornerIndex(x, y, 12);
      break;
    case 7:
      x += 8;
      y += 8;
      style = 18;
      break;
    case 12:
      x += 8;
      y += 16;
      style = 20;
      break;
    case 13:
      x += 8;
      y += 16;
      style = 19;
      break;
    case 14:
      style = 21;
      break;
    case 15:
      style = 22;
      break;
    case 16:
      style = 23;
      break;
    case 17:
      style = 24;
      break;
    case 18:
      x += 8;
      y += 8;
      style = 25;
      break;
    case 19:
      x += 8;
      y += 8;
      style = 26;
      break;
  }
  *xp = x;
  *yp = y;
  return style;
}
