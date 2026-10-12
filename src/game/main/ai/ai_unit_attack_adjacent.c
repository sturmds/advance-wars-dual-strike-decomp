#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/map_object.h"
#include "game/combatant.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s32 HwDiv32(s32 numerator, s32 denominator);
void Combat_CalcDamageToTarget(int unitIndex, int target, s16* pos);
int Unit_BestDamageToTargetKind(
    MapUnit* unit, int target); /* base damage percent against a target kind */
BOOL Map_IsTileVisible(s16 x, s16 y);
void Ai_ClearActingUnitTarget(void);
int AiAttack_Score(
    int arg,
    s16*
        pos); /* scores an attack on the unit slot in the low bits of arg from pos; 0: none */
MapObject* Map_GetObjectAt(int x, int y);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Attacks from where the acting unit stands, without moving. Like
 * AiUnit_PickDirectAttack, but looks only at the four neighbouring tiles
 * and omits the threat and "last open side" checks: queues an attack on the
 * best-scored visible enemy unit (action 4), and one on the object or hp
 * tile with the least hp (action 5). Used by AiFogDirect_ActOrder7. */
void AiUnit_AttackAdjacent(void) {
  /* needed to match: declaration order sets the stack slots and registers */
  int maxY;
  int maxX;
  int minX;
  int unitAttackX;
  int unitAttackY;
  int unitScore;
  int unitTargetSlot;
  int objectAttackX;
  int objectAttackY;
  s16 objectX;
  s16 objectY;
  int rating;
  MapObject* object;
  int minY;
  int hp;
  int objectHp;
  int targetKind;
  int y;
  int x;
  MapUnit* target;
  s16 pos[2];
  int dist;

  unitAttackX = -1;
  unitScore = 0;
  unitAttackY = -1;
  unitTargetSlot = 0;
  pos[0] = STATE->actingUnit->x;
  objectAttackX = -1;
  objectAttackY = -1;
  pos[1] = STATE->actingUnit->y;
  objectHp = 255;
  minX = STATE->actingUnit->x - 1;
  if (minX < 0) {
    minX = 0;
  }
  minY = STATE->actingUnit->y - 1;
  if (minY < 0) {
    minY = 0;
  }
  maxX = STATE->unk_1c4->mapWidth - 1;
  if (STATE->actingUnit->x + 1 < maxX) {
    maxX = STATE->actingUnit->x + 1;
  }
  maxY = STATE->unk_1c4->mapHeight - 1;
  if (STATE->actingUnit->y + 1 < maxY) {
    maxY = STATE->actingUnit->y + 1;
  }
  for (y = minY; y <= maxY; y++) {
    for (x = minX; x <= maxX; x++) {
      /* tiles at distance 1: the four sides of the unit */
      dist = 0;
      if (STATE->actingUnit->x - x > 0) {
        dist += STATE->actingUnit->x - x;
      } else {
        dist += x - STATE->actingUnit->x;
      }
      if (STATE->actingUnit->y - y > 0) {
        dist += STATE->actingUnit->y - y;
      } else {
        dist += y - STATE->actingUnit->y;
      }
      if (dist < 1 || dist > 1) {
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
      hp = STATE->tileHp[TILE(x, y)] & 0x7f;
      if (hp == 0 || STATE->mapTiles->unitAt[TILE(x, y)] != 0) {
        goto attackUnit;
      }
    attackObject:
      rating =
          HwDiv32(Unit_BestDamageToTargetKind(STATE->actingUnit, targetKind) *
                      STATE->actingUnit->hp,
                  100);
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
      rating = AiAttack_Score(STATE->mapTiles->unitAt[TILE(x, y)], pos);
      if (rating == 0 || rating < unitScore) {
        continue;
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
  if (objectHp == 255) {
    return;
  }
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(objectAttackX, objectAttackY, 5, objectX, objectY);
}
