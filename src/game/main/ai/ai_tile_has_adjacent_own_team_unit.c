#include "types.h"

int AiTile_HasOwnTeamUnit(int x, int y);

/* 1 when a unit of the current player's team stands on a tile next to (x, y). */
int AiTile_HasAdjacentOwnTeamUnit(int x, int y) {
  if (AiTile_HasOwnTeamUnit(x - 1, y) == 1) {
    return 1;
  }
  if (AiTile_HasOwnTeamUnit(x + 1, y) == 1) {
    return 1;
  }
  if (AiTile_HasOwnTeamUnit(x, y - 1) == 1) {
    return 1;
  }
  return AiTile_HasOwnTeamUnit(x, y + 1) == 1;
}
