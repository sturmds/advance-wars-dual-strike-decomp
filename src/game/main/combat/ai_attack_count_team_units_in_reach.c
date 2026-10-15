#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

int UnitType_GetAttackKind(u8 unitType);
int Player_GetUnitMoveRange(
    int player, int unitType); /* movement plus the CO's move bonus */
int AiAttack_CountTeamUnitsInFireRange(
    MapUnit* unit); /* the same count for an indirect unit */
BOOL Unit_CanBeAttackedBy(MapUnit* target, MapUnit* attacker);

#define TILE_INDEX(x, y) (MAP_STATE->mapTiles->rowStart[y] + (x))
#define MOVE_RANGE(unit) \
  Player_GetUnitMoveRange(MAP_UNIT_OWNER(MAP_STATE, unit), (unit)->type)

/* For a target unit that attacks directly: counts the current player's
 * team's units within the target's move range (distance in tiles) that
 * Unit_CanBeAttackedBy pairs with it, plus the acting unit. Air units count
 * twice. AiAttack_Score raises an attack's score by count / 32. */
int AiAttack_CountTeamUnitsInReach(MapUnit* unit) {
  int maxY;
  int maxX;
  int minY;
  int slot;
  MapUnit* other;
  int count = 0;
  int minX;
  int x;
  int y;
  int dist;

  if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_INDIRECT) {
    return AiAttack_CountTeamUnitsInFireRange(unit);
  }
  if (UnitType_GetAttackKind(unit->type) != ATTACK_KIND_DIRECT) {
    return count;
  }
  if (unit->x - MOVE_RANGE(unit) < 0) {
    minX = 0;
  } else {
    minX = unit->x - MOVE_RANGE(unit);
  }
  if (unit->y - MOVE_RANGE(unit) < 0) {
    minY = 0;
  } else {
    minY = unit->y - MOVE_RANGE(unit);
  }
  if (unit->x + MOVE_RANGE(unit) < MAP_STATE->unk_1c4->mapWidth - 1) {
    maxX = unit->x + MOVE_RANGE(unit);
  } else {
    maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  }
  if (unit->y + MOVE_RANGE(unit) < MAP_STATE->unk_1c4->mapHeight - 1) {
    maxY = unit->y + MOVE_RANGE(unit);
  } else {
    maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  }
  for (y = minY; y <= maxY; y++) {
    for (x = minX; x <= maxX; x++) {
      dist = 0;
      if (unit->x - x > 0) {
        dist += unit->x - x;
      } else {
        dist += x - unit->x;
      }
      if (unit->y - y > 0) {
        dist += unit->y - y;
      } else {
        dist += y - unit->y;
      }
      if (dist < 0 || dist > MOVE_RANGE(unit)) {
        continue;
      }
      slot = MAP_STATE->mapTiles->unitAt[TILE_INDEX(x, y)];
      if (slot == 0) {
        continue;
      }
      if (MAP_STATE->players[MAP_STATE->currentPlayer].team !=
          MAP_STATE->players[(slot >> 6) + 1].team) {
        continue;
      }
      other = &MAP_STATE->units[slot];
      if (Unit_CanBeAttackedBy(other, unit)) {
        count++;
        if (Ov000_gUnitStats[other->type].moveGroup & MOVE_GROUP_AIR) {
          count++;
        }
      }
    }
  }
  if (Unit_CanBeAttackedBy(MAP_STATE->actingUnit, unit) == TRUE) {
    count++;
    if (Ov000_gUnitStats[MAP_STATE->actingUnit->type].moveGroup &
        MOVE_GROUP_AIR) {
      count++;
    }
  }
  return count;
}
