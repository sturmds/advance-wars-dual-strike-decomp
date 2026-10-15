#include "types.h"
#include "game/Unk_02080440.h"
#include "game/keypad.h"

extern s16 data_02169454
    [DPAD_PATTERN_COUNT]
    [2]; /* per d-pad bit pattern (right, left, up, down): x step, y step */
extern s16 data_02169456[DPAD_PATTERN_COUNT]
                        [2]; /* the same table from its y steps */

void Sound_PlaySe(int se);

/* Like MapCursor_MoveByKeys without the range check: moves the map cursor
 * one tile by the d-pad keys in gKeypad->repeat, kept on the map, with
 * sound 0x10000 when it moves. */
void MapCursor_MoveByKeysFree(void) {
  int dir = (gKeypad->repeat >> 4) & 15;
  int se = 0;
  int newX = MAP_STATE->cursorX + data_02169454[dir][0];
  int newY;

  if (newX >= 0 && newX < MAP_STATE->unk_1c4->mapWidth) {
    MAP_STATE->cursorX = newX;
    if (data_02169454[dir][0] != 0) {
      se = 0x10000;
    }
  }
  newY = MAP_STATE->cursorY + data_02169456[dir][0];
  if (newY >= 0 && newY < MAP_STATE->unk_1c4->mapHeight) {
    MAP_STATE->cursorY = newY;
    if (data_02169456[dir][0] != 0) {
      se = 0x10000;
    }
  }
  if (se != 0) {
    Sound_PlaySe(se);
  }
}
