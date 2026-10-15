#include "types.h"
#include "game/Unk_02080440.h"
#include "game/keypad.h"

extern s16 data_02169454
    [DPAD_PATTERN_COUNT]
    [2]; /* per d-pad bit pattern (right, left, up, down): x step, y step */
extern s16 data_02169456[DPAD_PATTERN_COUNT]
                        [2]; /* the same table from its y steps */

void MapCursor_ClampToMap(void);

/* For the d-pad keys held in gKeypad->held: puts the cursor one tile in
 * from the edge of the visible area (16x12 tiles, or the map if smaller) on
 * each side it is pushed towards, runs MapCursor_ClampToMap, then steps it one
 * tile that way, kept on the map. The declaration order is needed to match. */
void MapCursor_StepPastViewEdge(void) {
  int dir;
  Unk_02080440_1c4* view;
  int newX;
  int width;
  int height;
  Unk_02080440_48* game;
  int newY;

  game = MAP_STATE;
  /* needed to match: this read adds the scaled index first */
  view = STATE_IDX_FIRST->unk_1c4;
  dir = (gKeypad->held >> 4) & 15;
  width = 16;
  height = 12;

  if (view->mapWidth < width) {
    width = view->mapWidth;
  }
  if (view->mapHeight < height) {
    height = view->mapHeight;
  }
  if (data_02169454[dir][0] < 0) {
    game->cursorX = (view->scrollX >> 4) + 1;
  }
  if (data_02169454[dir][0] > 0) {
    MAP_STATE->cursorX = width - 2 + (MAP_STATE->unk_1c4->scrollX >> 4);
  }
  if (data_02169456[dir][0] < 0) {
    MAP_STATE->cursorY = (MAP_STATE->unk_1c4->scrollY >> 4) + 1;
  }
  if (data_02169456[dir][0] > 0) {
    MAP_STATE->cursorY = height - 2 + (MAP_STATE->unk_1c4->scrollY >> 4);
  }
  MapCursor_ClampToMap();
  newX = MAP_STATE->cursorX + data_02169454[dir][0];
  if (newX >= 0 && newX < MAP_STATE->unk_1c4->mapWidth) {
    MAP_STATE->cursorX = newX;
  }
  newY = MAP_STATE->cursorY + data_02169456[dir][0];
  if (newY >= 0 && newY < MAP_STATE->unk_1c4->mapHeight) {
    MAP_STATE->cursorY = newY;
  }
}
