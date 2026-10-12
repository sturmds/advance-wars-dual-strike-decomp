#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int UnitType_GetAttackKind(int unitType);
BOOL Unit_IsCurrentPlayers(int unitSlot);
int Ai_BestDamage(
    int attackerType, int targetType,
    int arg); /* damage of an attacker type against a target type */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* TRUE when the acting direct-attack unit's hit on the target is weak (the
 * simulated battle in combatants costs the target under 30 hp) and a
 * ground teammate that has not acted yet, within 4 tiles of the target,
 * would do clearly more damage (hp times damage chart value, by more than
 * 2000). The callers then look for an attack position that leaves such a
 * teammate room. */
int AiUnit_TeammateOutdamages(MapUnit* target) {
  /* needed to match: declaration order, the two clamp forms below and the
     * row loop starting from y itself set the registers and stack slots */
  u8 slot;
  int y;
  int maxY;
  int maxX;
  MapUnit* other;
  int dist;
  int x;
  int minX;

  if (!(Ov000_gUnitStats[STATE->actingUnit->type].moveGroup &
        (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD |
         MOVE_GROUP_SEA))) {
    return 0;
  }
  if (UnitType_GetAttackKind(STATE->actingUnit->type) != ATTACK_KIND_DIRECT) {
    return 0;
  }
  if (STATE->combatants[1].hp == 0) {
    return 0;
  }
  if (STATE->combatants[1].hpLost < 30) {
    minX = target->x - 4;
    if (minX < 0) {
      minX = 0;
    }
    if (target->y - 4 < 0) {
      y = 0;
    } else {
      y = target->y - 4;
    }
    maxX = STATE->unk_1c4->mapWidth - 1;
    if (target->x + 4 < maxX) {
      maxX = target->x + 4;
    }
    maxY = STATE->unk_1c4->mapHeight - 1;
    if (target->y + 4 < maxY) {
      maxY = target->y + 4;
    }
    for (; y <= maxY; y++) {
      for (x = minX; x <= maxX; x++) {
        dist = 0;
        if (target->x - x > 0) {
          dist += target->x - x;
        } else {
          dist += x - target->x;
        }
        if (target->y - y > 0) {
          dist += target->y - y;
        } else {
          dist += y - target->y;
        }
        if (dist < 0 || dist > 4) {
          continue;
        }
        slot = STATE->mapTiles->unitAt[TILE(x, y)];
        if (slot == 0 || !Unit_IsCurrentPlayers(slot)) {
          continue;
        }
        if (x == STATE->actingUnit->x && y == STATE->actingUnit->y) {
          continue;
        }
        other = &STATE->units[slot];
        if (UnitType_GetAttackKind(other->type) == ATTACK_KIND_INDIRECT) {
          continue;
        }
        if (Ov000_gUnitStats[other->type].moveGroup & MOVE_GROUP_AIR) {
          continue;
        }
        if (other->flags & MAP_UNIT_WAITED) {
          continue;
        }
        if (other->type == UNIT_NONE) {
          continue;
        }
        if (other->captureProgress != 0) {
          continue;
        }
        if (STATE->actingUnit->hp *
                    Ai_BestDamage(STATE->actingUnit->type, target->type, 0) +
                2000 <
            other->hp * Ai_BestDamage(other->type, target->type, 0)) {
          return 1;
        }
      }
    }
  }
  return 0;
}
