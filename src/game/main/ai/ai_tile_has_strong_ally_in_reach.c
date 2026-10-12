#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_profile.h"
#include "game/ai_unit_action.h"
#include "game/unit_stats.h"
#include "game/range_map.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillMoves(int x, int y, int unitType, s16 movement, int arg4);
int UnitType_BestBaseDamage(int unitType, int targetType);
int UnitType_GetAttackKind(int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE(x, y) ((x) + MAP->rowStart[y])

/* TRUE when a same-team ally can reach (x, y) and hit the target: a direct
 * attacker that is not Infantry, Mech or Recon, with at least 60 hp, and
 * standing within the acting unit type's move range of (x, y) (the range
 * map is rebuilt around (x, y) in the scratch buffer rangeScratch and then
 * pointed back at rangeMapBuf). An ally that has already acted counts only
 * on a tile whose threatSum reaches the profile limit. */
BOOL AiTile_HasStrongAllyInReach(int x, int y, MapUnit* target) {
  MapUnit* ally;
  int col;
  int row;

  RangeMap_SetBuffer(MAP->rangeScratch);
  RangeMap_FillMoves(x, y, STATE->actingUnit->type, gAiTurnState->unk_05, 0);
  for (row = 0; row < STATE->unk_1c4->mapHeight; row++) {
    for (col = 0; col < STATE->unk_1c4->mapWidth; col++) {
      if (STATE->rangeMap[row][col] < 0) {
        continue;
      }
      ally = &STATE->units[MAP->unitAt[TILE(col, row)]];
      if (ally->type == UNIT_NONE) {
        continue;
      }
      if (ally == STATE->actingUnit) {
        continue;
      }
      if (STATE->players[(STATE->actingUnitIndex >> 6) + 1].team !=
          STATE->players[MAP_UNIT_OWNER(STATE, ally)].team) {
        continue;
      }
      if (UnitType_BestBaseDamage(ally->type, target->type) == 0) {
        continue;
      }
      if (UnitType_GetAttackKind(ally->type) != ATTACK_KIND_DIRECT) {
        continue;
      }
      if (ally->type <= UNIT_MECH || ally->type == UNIT_RECON) {
        continue;
      }
      if (ally->hp < 60) {
        continue;
      }
      if (ally->flags & MAP_UNIT_WAITED) {
        if (MAP->threatSum[TILE(col, row)] <
            gAiActiveUnitProfile->threatSumLimit) {
          continue;
        }
      }
      RangeMap_SetBuffer(MAP->rangeMapTiles);
      return TRUE;
    }
  }
  RangeMap_SetBuffer(MAP->rangeMapTiles);
  return FALSE;
}
