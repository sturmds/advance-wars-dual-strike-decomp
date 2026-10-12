#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_profile.h"
#include "game/map_unit.h"
#include "game/terrain.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Ai_CanEndMoveAt(s16 x, s16 y);
int Ai_IsTileNeededByAlly(s16 x, s16 y);
int UnitType_GetAttackKind(int unitType);
BOOL Unit_IsEnemyTile(int unitIndex, int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* TRUE when the acting unit may end its move on (x, y) on the way to a
 * target (AiUnit_MoveTowardTile). The tile's threatMax and threatSum must
 * stay below the limits in gAiActiveUnitProfile, Ai_CanEndMoveAt must
 * allow it and Ai_IsTileNeededByAlly must not claim it. An armed unit
 * that does not move by sea may not stop on ports unless they are enemy-owned,
 * and a unit waiting for a T-Copter (transportWanted 2) may not stop on ports
 * and shoals. */
BOOL AiTile_IsSafeStop(s16 x, s16 y) {
  /* needed to match: the first read adds the scaled index first */
  int tile = x + STATE_IDX_FIRST->mapTiles->rowStart[y];

  if (STATE->mapTiles->threatMax[tile] >=
      gAiActiveUnitProfile->threatMaxLimit) {
    return FALSE;
  }
  if (STATE->mapTiles->threatSum[tile] >=
      gAiActiveUnitProfile->threatSumLimit) {
    return FALSE;
  }
  if (!Ai_CanEndMoveAt(x, y)) {
    return FALSE;
  }
  if (Ov000_gUnitStats[STATE->actingUnit->type].moveGroup != MOVE_GROUP_SEA &&
      UnitType_GetAttackKind(STATE->actingUnit->type) != ATTACK_KIND_NONE &&
      MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(x, y)]) == TERRAIN_PORT) {
    if (!Unit_IsEnemyTile(STATE->actingUnitIndex, x, y)) {
      return FALSE;
    }
  }
  if (Ai_IsTileNeededByAlly(x, y) == 1) {
    return FALSE;
  }
  if (STATE->actingUnit->transportWanted == 2) {
    int terrain = MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(x, y)]);
    if (terrain == TERRAIN_PORT || terrain == TERRAIN_SHOAL) {
      return FALSE;
    }
  }
  return TRUE;
}
