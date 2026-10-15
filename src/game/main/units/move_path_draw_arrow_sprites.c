#include "types.h"
#include "game/move_path.h"
#include "game/Unk_02080440.h"
#include "game/oam_cells.h"

extern MovePath* gMovePath;

int MovePath_DirToNext(int i);
int MovePath_DirToPrev(int i);
BOOL Rect_IsOnScreen(int x, int y, int width, int height);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Queues the move path arrow of every on-screen tile as a sprite, last tile
 * first; the sprite version of MovePath_DrawArrows3d. Nothing is drawn while
 * the path is only the unit's own tile. */
void MovePath_DrawArrowSprites(void) {
  s8 i;
  s16 x;
  s16 y;
  int next;
  int tile;
  Unk_02080440_1c4* view;

  if (gMovePath->lastTile == 0) {
    return;
  }
  for (i = gMovePath->lastTile; i >= 0; i--) {
    x = gMovePath->x[i] << 4;
    y = gMovePath->y[i] << 4;
    if (Rect_IsOnScreen(x, y, 16, 16)) {
      tile = (u8)i; /* needed to match: an int copy, set before view */
      view = MAP_STATE->unk_1c4;
      next = MovePath_DirToNext(tile);
      /* arrow cells start at OBJ tile 0x194; palette 3 */
      Sprite_QueueAt(
          3, (x - view->scrollX) & 0x1ff, (y - view->scrollY) & 0xff,
          &data_027c0040,
          (gMovePathArrowCells[next][MovePath_DirToPrev(tile)] + 0x194) |
              0x3000,
          0);
    }
  }
}
