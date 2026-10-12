#include "types.h"
#include "game/Unk_02080440.h"
#include "game/move_path.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Walks the unit's path from (startX, startY). At the first tile that holds a
 * unit of another team (a hidden unit in fog), cuts the path there with
 * PATH_STEP_BLOCKED and `end`, moves the move's destination to the tile
 * before it and returns 1. Returns 0 when nothing blocks the path, and right
 * away for Oozium, which moves onto enemy units. */
int UnitPath_StopAtEnemy(int startX, int startY, int unitType, s8* path,
                         int end) {
  s16 x = startX;
  s16 y = startY;
  s16 curX = x;
  s16 curY = y;
  u8 slot;

  if (unitType == UNIT_OOZIUM) {
    return 0;
  }
  while (*path != PATH_STEP_END && *path != -1) {
    switch (*path) {
      case PATH_STEP_LEFT:
        curX--;
        break;
      case PATH_STEP_RIGHT:
        curX++;
        break;
      case PATH_STEP_DOWN:
        curY++;
        break;
      case PATH_STEP_UP:
        curY--;
        break;
    }
    slot = GAME->mapTiles->unk_508[GAME->mapTiles->rowStart[curY] + curX];
    if (slot != 0 && GAME->players[(slot >> 6) + 1].team !=
                         GAME->players[GAME->currentPlayer].team) {
      path[0] = PATH_STEP_BLOCKED;
      path[1] = end;
      GAME->actingUnitDestX = x;
      GAME->actingUnitDestY = y;
      GAME->unk_46 = GAME->rangeMap[y][x];
      return 1;
    }
    x = curX;
    y = curY;
    path++;
  }
  return 0;
}
