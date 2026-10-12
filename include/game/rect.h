#ifndef GAME_RECT_H
#define GAME_RECT_H

#include "types.h"

/* A screen rectangle in pixels, 8 bytes. */
typedef struct Rect {
  s16 x;
  s16 y;
  s16 width;
  s16 height;
} Rect;

BOOL Rect_ContainsPoint(int x, int y, const Rect* rect);

#endif
