#include "types.h"
#include "game/Unk_02080440.h"
#include "game/keypad.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
/* needed to match: with a length, mwcc reads the steps again after each store */
extern s16 data_02169454
    [DPAD_PATTERN_COUNT]
    [2]; /* per d-pad bit pattern (right, left, up, down): x step, y step */
extern s16 data_02169456[DPAD_PATTERN_COUNT]
                        [2]; /* the same table from its y steps */

void Sound_PlaySe(int se);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Moves the map cursor (cursorX, cursorY) one tile in the direction of the
 * d-pad keys in gKeypad->repeat, kept on the map, with sound 0x10000 when it
 * moves. */
void MapCursor_MoveByKeys(void) {
  Unk_02080440_48* game = GAME;
  int dir = (gKeypad->repeat >> 4) & 15;
  int newX = game->cursorX + data_02169454[dir][0];
  s16 nx = newX;
  /* needed to match: this read adds the scaled index first */
  MapTiles* tiles = STATE_IDX_FIRST->mapTiles;
  int se = 0;
  s16 ny = game->cursorY + data_02169456[dir][0];
  int newY;
  s8 range;

  /* Inside the reach shown for a unit, auto-repeat stops at its edge;
     * a new press steps out. */
  if (tiles->rangeMapTiles[game->cursorX + tiles->rowStart[game->cursorY]] >
      0) {
    range = tiles->rangeMapTiles[nx + tiles->rowStart[ny]];
    if (range < 0 || range > 110) {
      if (gKeypad->repeat != gKeypad->pressed) {
        return;
      }
    }
  }
  if (newX >= 0 && newX < GAME->unk_1c4->mapWidth) {
    GAME->cursorX = newX;
    if (data_02169454[dir][0] != 0) {
      se = 0x10000;
    }
  }
  newY = GAME->cursorY + data_02169456[dir][0];
  if (newY >= 0 && newY < GAME->unk_1c4->mapHeight) {
    GAME->cursorY = newY;
    if (data_02169456[dir][0] != 0) {
      se = 0x10000;
    }
  }
  if (se != 0) {
    Sound_PlaySe(se);
  }
}
