#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/map_object.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Unit_BestDamageToTargetKind(MapUnit* unit, int target);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(int arg);
MapObject* Map_GetObjectAt(int x, int y);
int AiTile_GetLowestAdjacentReach(int x, int y);
BOOL AiUnit_FindFiringTile(s16* pos);
void AiUnit_MoveTowardTile(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* For a unit that does at least 10 damage to target kind 4: picks the
 * reachable tile with the lowest AiTile_GetLowestAdjacentReach score that holds a tile
 * with hp or a map object (as gAiTurnState->flags allows), and, if
 * AiUnit_FindFiringTile accepts it, moves toward it with AiUnit_MoveTowardTile. */
void AiUnit_HeadForObjectTarget(void) {
  int bestScore = 0x7fff;
  s16 pos[2];
  int score;
  int x;
  int y;

  if (Unit_BestDamageToTargetKind(STATE->actingUnit, 4) < 10) {
    return;
  }
  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                     STATE->actingUnit->type, 110, 1);
  RangeMap_MarkAdjacentTiles(111);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if ((STATE->tileHp[TILE(x, y)] & 0x7f) != 0) {
        if (!AI_TARGET_FLAGS->attackTiles) {
          continue;
        }
      } else if (!AI_TARGET_FLAGS->attackObjects ||
                 Map_GetObjectAt(x, y) == NULL) {
        continue;
      }
      score = AiTile_GetLowestAdjacentReach(x, y);
      if (score < bestScore) {
        bestScore = score;
        pos[0] = x;
        pos[1] = y;
      }
    }
  }
  if (bestScore == 0x7fff) {
    return;
  }
  if (!AiUnit_FindFiringTile(pos)) {
    return;
  }
  AiUnit_MoveTowardTile(pos[0], pos[1]);
}
