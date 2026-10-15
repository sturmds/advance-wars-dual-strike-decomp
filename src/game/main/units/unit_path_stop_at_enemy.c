#include "types.h"
#include "game/Unk_02080440.h"
#include "game/move_path.h"
#include "game/unit_stats.h"

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
    slot = MAP_STATE->mapTiles
               ->unk_508[MAP_STATE->mapTiles->rowStart[curY] + curX];
    if (slot != 0 && MAP_STATE->players[(slot >> 6) + 1].team !=
                         MAP_STATE->players[MAP_STATE->currentPlayer].team) {
      path[0] = PATH_STEP_BLOCKED;
      path[1] = end;
      MAP_STATE->actingUnitDestX = x;
      MAP_STATE->actingUnitDestY = y;
      MAP_STATE->unk_46 = MAP_STATE->rangeMap[y][x];
      return 1;
    }
    x = curX;
    y = curY;
    path++;
  }
  return 0;
}
