#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int UnitType_GetAttackKind(int unitType);
int GetUnitBaseMaxRange(int player, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define NO_TILE 9999
#define MAX_RANGE() \
  GetUnitBaseMaxRange(STATE->currentPlayer, STATE->actingUnit->type)

/* For an indirect unit about to attack the tile in pos: replaces pos with
 * the reachable tile, 0 to 110 move cost, that is in the unit's firing range
 * of it, nearest first and then cheapest to reach. Returns FALSE when no
 * tile qualifies. Other unit kinds keep pos and return TRUE. */
BOOL AiUnit_FindFiringTile(s16* pos) {
  int xMin;
  int cost;
  int bestX;
  int bestDist;
  int y;
  int xMax;
  int yMax;
  int x;
  int dist;
  int bestY;

  if (UnitType_GetAttackKind(STATE->actingUnit->type) != ATTACK_KIND_INDIRECT) {
    return TRUE;
  }
  bestY = NO_TILE;
  bestX = NO_TILE;
  bestDist = MAX_RANGE() + 1;
  xMin = pos[0] - MAX_RANGE() < 0 ? 0 : pos[0] - MAX_RANGE();
  y = pos[1] - MAX_RANGE() < 0 ? 0 : pos[1] - MAX_RANGE();
  xMax = pos[0] + MAX_RANGE() < STATE->unk_1c4->mapWidth - 1
             ? pos[0] + MAX_RANGE()
             : STATE->unk_1c4->mapWidth - 1;
  yMax = pos[1] + MAX_RANGE() < STATE->unk_1c4->mapHeight - 1
             ? pos[1] + MAX_RANGE()
             : STATE->unk_1c4->mapHeight - 1;
  for (; y <= yMax; y++) {
    for (x = xMin; x <= xMax; x++) {
      dist = 0;
      if (pos[0] - x > 0) {
        dist += pos[0] - x;
      } else {
        dist += x - pos[0];
      }
      if (pos[1] - y > 0) {
        dist += pos[1] - y;
      } else {
        dist += y - pos[1];
      }
      if (dist < Ov000_gUnitStats[STATE->actingUnit->type].minRange) {
        continue;
      }
      if (dist > MAX_RANGE() || dist > bestDist) {
        continue;
      }
      cost = STATE->rangeMap[y][x];
      if (cost < 0 || cost > 110) {
        continue;
      }
      if (bestX != NO_TILE && dist == bestDist &&
          cost > STATE->rangeMap[bestY][bestX]) {
        continue;
      }
      bestDist = dist;
      bestX = x;
      bestY = y;
    }
  }
  if (bestX == NO_TILE) {
    return FALSE;
  }
  pos[0] = bestX;
  pos[1] = bestY;
  return TRUE;
}
