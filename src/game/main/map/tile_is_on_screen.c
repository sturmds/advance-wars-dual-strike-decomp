#include "types.h"
#include "game/Unk_02080440.h"

/* TRUE when tile (x, y) is inside the part of the map the screen shows now:
 * 32 by 24 tiles from the view scroll (1/16 tile units). */
BOOL Tile_IsOnScreen(int x, int y) {
  Unk_02080440_1c4* view = MAP_STATE->unk_1c4;

  if (x < (view->scrollX >> 4) || y < (view->scrollY >> 4) ||
      x >= (view->scrollX >> 4) + 32 || y >= (view->scrollY >> 4) + 24) {
    return FALSE;
  }
  return TRUE;
}
