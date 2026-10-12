#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int AiTile_HasEnemyOozium(int x, int y);
int AiTile_HasAdjacentOwnTeamUnit(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* 1 when an enemy Oozium stands next to (x, y) and no unit of the current
 * player's team stands next to that Oozium. Always 0 while the current
 * player's tag power runs its front half. The AiSite_Pick* functions skip
 * sites where this is 1. */
int AiSite_NearUnguardedOozium(int x, int y) {
  if (STATE->players[STATE->currentPlayer].tagPowerHalf == TAG_POWER_FRONT) {
    return 0;
  }
  if (AiTile_HasEnemyOozium(x - 1, y) == 1 &&
      AiTile_HasAdjacentOwnTeamUnit(x - 1, y) == 0) {
    return 1;
  }
  if (AiTile_HasEnemyOozium(x + 1, y) == 1 &&
      AiTile_HasAdjacentOwnTeamUnit(x + 1, y) == 0) {
    return 1;
  }
  if (AiTile_HasEnemyOozium(x, y - 1) == 1 &&
      AiTile_HasAdjacentOwnTeamUnit(x, y - 1) == 0) {
    return 1;
  }
  if (AiTile_HasEnemyOozium(x, y + 1) == 1 &&
      AiTile_HasAdjacentOwnTeamUnit(x, y + 1) == 0) {
    return 1;
  }
  return 0;
}
