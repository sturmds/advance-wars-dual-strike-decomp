#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int AiActingUnit_GetSearchMovement(void);
void AiActingUnit_MarkAttackRange(int movement);
BOOL AiUnit_FindFiringTile(s16* pos);
BOOL func_020a3dfc(int unitType, int tile);
BOOL Unit_IsTileOwnedByTeam(int unitIndex, int tile);
void AiUnit_MoveTowardTile(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Moves the acting unit toward the reachable tile with the lowest rangeMap
 * value (the last one on a tie) whose owner is another team's and that
 * func_020a3dfc accepts for the unit type, if AiUnit_FindFiringTile agrees to the
 * tile in pos. AiActingUnit_MarkAttackRange runs first with the search movement. */
void AiUnit_MoveToEnemyProperty(void) {
  s16 pos[2];
  int y;
  int tile;
  int bestReach = 0x7fff;
  int x;

  AiActingUnit_MarkAttackRange(AiActingUnit_GetSearchMovement());
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      tile = STATE->mapTiles->tiles[TILE(x, y)];
      if ((tile & 0xe0) == 0) {
        continue;
      }
      if (Unit_IsTileOwnedByTeam((s16)STATE->currentUnitBase, (u8)tile) == 1) {
        continue;
      }
      if (!func_020a3dfc(STATE->actingUnit->type, tile)) {
        continue;
      }
      if (bestReach < STATE->rangeMap[y][x]) {
        continue;
      }
      pos[0] = x;
      pos[1] = y;
      bestReach = STATE->rangeMap[y][x];
    }
  }
  if (bestReach == 0x7fff) {
    return;
  }
  if (!AiUnit_FindFiringTile(pos)) {
    return;
  }
  AiUnit_MoveTowardTile(pos[0], pos[1]);
}
