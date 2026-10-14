#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u16 data_0229ead8[];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Moves the drawn cursor position (map pixels) toward (x, y): 4 pixels per
 * call on each axis, or straight there when it is more than 16 pixels away on
 * either axis. The position is STATE->cursorPixelX/cursorPixelY, or data_0229ead8 for
 * mode 3 (PopupMenu_DestroyCommon sets it). Writes the new position to outX/outY. */
void MapCursor_SlideToward(s16 x, s16 y, s16 mode, s16* outX, s16* outY) {
  u16* pos;
  u16 posX;
  u16 posY;
  int dx;
  int dy;
  int step;

  if (mode == 3) {
    pos = data_0229ead8;
  } else {
    pos = &STATE->cursorPixelX;
  }
  posX = pos[0];
  posY = pos[1];
  /* needed to match: dx and dy are assigned inside the test */
  if (((dx = x - posX) < 0 ? -dx : dx) > 16 ||
      ((dy = y - posY) < 0 ? -dy : dy) > 16) {
    posX = x;
    posY = y;
  } else {
    if ((dx < 0 ? -dx : dx) > 0) {
      if (dx > 0) {
        step = 1;
      } else {
        step = -(dx < 0);
      }
      posX = posX + step * 4;
    }
    if ((dy < 0 ? -dy : dy) > 0) {
      if (dy > 0) {
        step = 1;
      } else {
        step = -(dy < 0);
      }
      posY = posY + step * 4;
    }
  }
  pos[0] = (s16)posX;
  pos[1] = (s16)posY;
  *outX = (s16)posX;
  *outY = (s16)posY;
}
