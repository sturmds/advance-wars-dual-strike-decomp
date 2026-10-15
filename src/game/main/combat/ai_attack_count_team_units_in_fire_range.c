#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

int GetUnitMaxRange(int player, int unitType);
BOOL Unit_CanBeAttackedBy(MapUnit* target, MapUnit* attacker);

#define TILE_INDEX(x, y) (MAP_STATE->mapTiles->rowStart[y] + (x))
#define MAX_RANGE(unit) \
  GetUnitMaxRange(MAP_UNIT_OWNER(MAP_STATE, unit), (unit)->type)

/* For a target unit that attacks indirectly: counts the current player's
 * team's units on tiles between the target's minimum range and its maximum
 * range (distance in tiles) that Unit_CanBeAttackedBy pairs with it, times 3. */
int AiAttack_CountTeamUnitsInFireRange(MapUnit* unit) {
  int maxY;
  int maxX;
  int minY;
  int minX;
  int slot;
  int count = 0;
  int x;
  int y;
  int dist;

  if (unit->x - MAX_RANGE(unit) < 0) {
    minX = 0;
  } else {
    minX = unit->x - MAX_RANGE(unit);
  }
  if (unit->y - MAX_RANGE(unit) < 0) {
    minY = 0;
  } else {
    minY = unit->y - MAX_RANGE(unit);
  }
  if (unit->x + MAX_RANGE(unit) < MAP_STATE->unk_1c4->mapWidth - 1) {
    maxX = unit->x + MAX_RANGE(unit);
  } else {
    maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  }
  if (unit->y + MAX_RANGE(unit) < MAP_STATE->unk_1c4->mapHeight - 1) {
    maxY = unit->y + MAX_RANGE(unit);
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
      if (dist < Ov000_gUnitStats[unit->type].minRange ||
          dist > MAX_RANGE(unit)) {
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
      if (Unit_CanBeAttackedBy(&MAP_STATE->units[slot], unit)) {
        count++;
      }
    }
  }
  return count * 3;
}
