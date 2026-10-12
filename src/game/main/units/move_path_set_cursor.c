#include "types.h"
#include "game/move_path.h"

extern MovePath* gMovePath;

void MovePath_SetCursor(int x, int y) {
  gMovePath->cursorX = x;
  gMovePath->cursorY = y;
}
