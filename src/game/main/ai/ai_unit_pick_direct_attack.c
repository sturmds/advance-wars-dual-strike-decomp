#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_profile.h"
#include "game/ai_unit_action.h"
#include "game/map_object.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s32 HwDiv32(s32 numerator, s32 denominator);
void Combat_CalcDamageToTarget(int unitIndex, int target, s16* pos);
int UnitType_GetAttackKind(int unitType);
void RangeMap_FillUnitMoves(MapUnit* unit);
void RangeMap_MarkAdjacentTiles(int arg);
void AiUnit_PerformAction(
    s16 x, s16 y, int action, u8 arg3,
    u8 arg4); /* queues the move to (x, y) and the action */
int Ai_CountReachableAdjacentTiles(int x, int y);
int Unit_BestDamageToTargetKind(
    MapUnit* unit, int target); /* base damage percent against a target kind */
BOOL Map_IsTileVisible(s16 x, s16 y);
int AiUnit_CruiserCanReachSub(void);
BOOL AiTile_HasStrongAllyInReach(int x, int y, MapUnit* target);
void Ai_ClearActingUnitTarget(void);
int AiUnit_TeammateOutdamages(MapUnit* target);
BOOL Ai_CanEndMoveAt(int x, int y);
BOOL FindAttackTile(
    int x, int y,
    s16* pos); /* finds a tile to attack (x, y) from; FALSE: none */
int AiAttack_Score(
    int arg,
    s16*
        pos); /* scores an attack on the unit slot in the low bits of arg from pos; 0: none */
void AiUnit_MoveToSafestTile(void);
MapObject* Map_GetObjectAt(
    int x, int y); /* object standing at (x, y); NULL: none or destroyed */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Picks what the AI's acting direct-attack unit hits this turn and queues it:
 * the best-scored enemy unit (action 4) and an object or hp tile, preferring
 * low hp (action 5). Called from the AI unit actions through
 * AiUnit_PlanAttack when UnitType_GetAttackKind gives 0. */
void AiUnit_PickDirectAttack(void) {
  /* needed to match: declaration order sets the stack slots */
  int maxY;
  int minX;
  int unitAttackX;
  int unitAttackY;
  int unitScore;
  int onTargetSide;
  int holdsLastSide;
  int unitTargetSlot;
  int objectAttackX;
  int objectAttackY;
  int objectHp;
  int flags;
  int x;
  int y;
  s16 objectX;
  s16 objectY;
  int hp;
  int targetKind;
  int rating; /* needed to match: damage to an object, then the score of a unit attack */
  int aiBehaviour;
  int tileFlag;
  MapObject* object;
  MapUnit* target;
  MapUnit* neighbour;
  s16 pos[2];
  int maxX;
  int minY;
  int openSides;
  int xx;
  int yy;
  int dist;
  int tile;

  unitAttackX = -1;
  unitScore = 0;
  unitAttackY = -1;
  holdsLastSide = 0;
  unitTargetSlot = 0;
  objectAttackX = -1;
  objectAttackY = -1;
  objectHp = 255;
  if (STATE->unk_1c4->mapId == 238) {
    flags = 0;
  } else if (AiUnit_CruiserCanReachSub() == 1) {
    flags = AI_ATTACK_NO_CHIP;
  } else {
    flags = 0;
  }
  RangeMap_FillUnitMoves(STATE->actingUnit);
  RangeMap_MarkAdjacentTiles(111);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (AI_TARGET_FLAGS->attackObjects) {
        object = Map_GetObjectAt(x, y);
        if (object != NULL) {
          hp = object->hp;
          targetKind = object->kind;
          goto attackObject;
        }
      }
      if (!AI_TARGET_FLAGS->attackTiles) {
        goto attackUnit;
      }
      targetKind = 6;
      tile = TILE(x, y);
      hp = STATE->tileHp[tile] & 0x7f;
      if (hp == 0 || STATE->mapTiles->unitAt[tile] != 0) {
        goto attackUnit;
      }
    attackObject:
      rating =
          HwDiv32(Unit_BestDamageToTargetKind(STATE->actingUnit, targetKind) *
                      STATE->actingUnit->hp,
                  100);
      /* skip unless it destroys the object or deals at least 10 */
      if (rating < hp && rating < 10) {
        continue;
      }
      if (!FindAttackTile(x, y, pos)) {
        continue;
      }
      Combat_CalcDamageToTarget(STATE->actingUnitIndex, targetKind, pos);
      if (STATE->combatants[0].weapon == COMBAT_WEAPON_NONE) {
        continue;
      }
      if (rating >= objectHp) {
        if (rating < hp) {
          continue;
        }
        if (objectHp > hp) {
          continue;
        }
      } else if (objectHp < hp) {
        continue;
      }
      aiBehaviour = STATE->players[STATE->currentPlayer].aiBehaviour;
      if (aiBehaviour == AI_BEHAVIOUR_DEFENSE && rating < hp &&
          STATE->actingUnit->type > 2) {
        tile = TILE(pos[0], pos[1]);
        if (STATE->mapTiles->threatMax[tile] >=
                gAiActiveUnitProfile->threatMaxLimit ||
            STATE->mapTiles->threatSum[tile] >=
                gAiActiveUnitProfile->threatSumLimit) {
          continue;
        }
      }
      if (aiBehaviour == AI_BEHAVIOUR_GENERAL &&
          Ai_CountReachableAdjacentTiles(unitAttackX, unitAttackY) >= 2 &&
          STATE->actingUnit->type > 2) {
        if (STATE->mapTiles->threatSum[TILE(pos[0], pos[1])] >=
            gAiActiveUnitProfile->threatSumLimit) {
          continue;
        }
      }
      if (!Ai_CanEndMoveAt(pos[0], pos[1])) {
        continue;
      }
      objectHp = hp;
      objectAttackX = pos[0];
      objectAttackY = pos[1];
      objectX = x;
      objectY = y;
      continue;

    attackUnit:
      target = &STATE->units[STATE->mapTiles->unitAt[TILE(x, y)]];
      if (target->type == 0) {
        continue;
      }
      if (STATE->players[(STATE->actingUnitIndex >> 6) + 1].team ==
          STATE->players[MAP_UNIT_OWNER(STATE, target)].team) {
        continue;
      }
      if (!Map_IsTileVisible(x, y)) {
        continue;
      }
      if (!FindAttackTile(x, y, pos)) {
        continue;
      }
      tileFlag = 0;
      tile = TILE(pos[0], pos[1]);
      if (STATE->mapTiles->threatSum[tile] >=
              gAiActiveUnitProfile->threatSumLimit ||
          (STATE->mapTiles->threatMax[tile] >=
               gAiActiveUnitProfile->threatMaxLimit &&
           target->cargo[0] == 0 &&
           UnitType_GetAttackKind(target->type) == 2)) {
        if (!AiTile_HasStrongAllyInReach(pos[0], pos[1], target)) {
          tileFlag = AI_ATTACK_COST_VALUE;
        }
      }
      rating = AiAttack_Score(
          flags | (tileFlag | STATE->mapTiles->unitAt[TILE(x, y)]), pos);
      if (rating == 0 || rating < unitScore) {
        continue;
      }
      if (AiUnit_TeammateOutdamages(target) == 1) {
        openSides = 0;
        onTargetSide = 0;
        minX = target->x - 1;
        if (minX < 0) {
          minX = 0;
        }
        minY = target->y - 1;
        if (minY < 0) {
          minY = 0;
        }
        maxX = STATE->unk_1c4->mapWidth - 1;
        if (target->x + 1 < maxX) {
          maxX = target->x + 1;
        }
        maxY = STATE->unk_1c4->mapHeight - 1;
        if (target->y + 1 < maxY) {
          maxY = target->y + 1;
        }
        for (yy = minY; yy <= maxY; yy++) {
          for (xx = minX; xx <= maxX; xx++) {
            /* tiles at distance 1: the target's four sides */
            dist = 0;
            if (target->x - xx > 0) {
              dist += target->x - xx;
            } else {
              dist += xx - target->x;
            }
            if (target->y - yy > 0) {
              dist += target->y - yy;
            } else {
              dist += yy - target->y;
            }
            if (dist < 1 || dist > 1) {
              continue;
            }
            if (STATE->rangeMap[yy][xx] < 0) {
              continue;
            }
            neighbour = &STATE->units[STATE->mapTiles->unitAt[TILE(xx, yy)]];
            if (neighbour->type != 0 && (neighbour->flags & MAP_UNIT_WAITED)) {
              continue;
            }
            openSides++;
            if (xx == STATE->actingUnit->x && yy == STATE->actingUnit->y) {
              onTargetSide = 1;
            }
          }
        }
        /* one open side or none: leave the target, and note when the
                 * acting unit stands on that last side */
        if (openSides <= 1) {
          if (onTargetSide) {
            holdsLastSide = 1;
          }
          continue;
        }
      }
      unitScore = rating;
      unitAttackX = pos[0];
      unitAttackY = pos[1];
      unitTargetSlot = STATE->mapTiles->unitAt[TILE(x, y)];
    }
  }
  if (unitScore != 0) {
    Ai_ClearActingUnitTarget();
    AiUnit_PerformAction(unitAttackX, unitAttackY, 4, unitTargetSlot, 0);
  }
  if (objectHp != 255) {
    Ai_ClearActingUnitTarget();
    AiUnit_PerformAction(objectAttackX, objectAttackY, 5, objectX, objectY);
  }
  if (holdsLastSide) {
    AiUnit_MoveToSafestTile();
  }
}
