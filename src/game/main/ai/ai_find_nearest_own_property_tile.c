#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_profile.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int UnitType_GetAttackKind(int unitType);
BOOL Terrain_IsProperty(u8 terrain);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
BOOL Ai_HasEnemyWithinMoves(int x, int y, int arg2, int arg3);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])
#define TILE(x, y) (MAP->tiles[TILE_INDEX(x, y)])

#define NO_TILE 0x7fff
#define FAR_AWAY 110

/* For a land or air unit: finds the nearest reachable property tile of the
 * current player that Ai_HasEnemyWithinMoves accepts, and writes its x and y to
 * out. An indirect unit skips tiles whose enemy threat sum has reached its
 * profile's limit. Returns FALSE when there is none. */
BOOL Ai_FindNearestOwnPropertyTile(s16* out) {
  int bestX = -1;
  int bestY = -1;
  int best = NO_TILE;
  int x;
  int y;

  if (Ov000_gUnitStats[STATE->actingUnit->type].moveGroup & MOVE_GROUP_SEA) {
    return FALSE;
  }
  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                     STATE->actingUnit->type, FAR_AWAY, 0);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (!Terrain_IsProperty(MAP_TILE_TERRAIN(TILE(x, y)))) {
        continue;
      }
      if (STATE->unk_04 != (TILE(x, y) & 0xe0)) {
        continue;
      }
      if (best < STATE->rangeMap[y][x]) {
        continue;
      }
      if (!Ai_HasEnemyWithinMoves(x, y, 1, 5)) {
        continue;
      }
      if (UnitType_GetAttackKind(STATE->actingUnit->type) ==
          ATTACK_KIND_INDIRECT) {
        if (MAP->threatSum[TILE_INDEX(x, y)] >=
            gAiActiveUnitProfile->threatSumLimit) {
          continue;
        }
      }
      bestX = x;
      bestY = y;
      best = STATE->rangeMap[y][x];
    }
  }
  if (best == NO_TILE) {
    return FALSE;
  }
  out[0] = bestX;
  out[1] = bestY;
  return TRUE;
}
