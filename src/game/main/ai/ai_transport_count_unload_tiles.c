#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int AiTile_IsFreeForUnitType(int x, int y, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sets count to the number of the four tiles next to (x, y) that a unit of
 * unitType could get off onto. */
#define COUNT_OPEN_NEIGHBOURS(count, x, y, unitType)           \
  count = AiTile_IsFreeForUnitType((x) - 1, (y), (unitType));  \
  count += AiTile_IsFreeForUnitType((x) + 1, (y), (unitType)); \
  count += AiTile_IsFreeForUnitType((x), (y) - 1, (unitType)); \
  count += AiTile_IsFreeForUnitType((x), (y) + 1, (unitType))

/* Whether the acting transport could unload its cargo at (x, y), as a count
 * of open tiles next to it; 0 means no. With one unit aboard, counts the
 * tiles open to that unit's type. With two, Tank tiles stand for any
 * vehicle and Infantry tiles for foot units: two vehicles count Tank tiles;
 * a vehicle in the second slot (first slot foot) falls back to 1 when only
 * Infantry tiles are open; a vehicle in the first slot (second slot foot)
 * needs two Tank tiles, else it falls back the same way; two foot units
 * count Infantry tiles. */
int AiTransport_CountUnloadTiles(int x, int y) {
  MapUnit* first;
  MapUnit* second;
  int count;

  second = &STATE->units[STATE->actingUnit->cargo[1]];
  first = &STATE->units[STATE->actingUnit->cargo[0]];
  if (second->type == UNIT_NONE) {
    COUNT_OPEN_NEIGHBOURS(count, x, y, first->type);
    return count;
  }
  if (second->type > UNIT_MECH) {
    if (first->type > UNIT_MECH) {
      COUNT_OPEN_NEIGHBOURS(count, x, y, UNIT_TANK);
      return count;
    }
    COUNT_OPEN_NEIGHBOURS(count, x, y, UNIT_TANK);
    if (count != 0) {
      return count;
    }
    COUNT_OPEN_NEIGHBOURS(count, x, y, UNIT_INFANTRY);
    return count != 0;
  }
  if (first->type > UNIT_MECH) {
    COUNT_OPEN_NEIGHBOURS(count, x, y, UNIT_TANK);
    if (count >= 2) {
      return count;
    }
    COUNT_OPEN_NEIGHBOURS(count, x, y, UNIT_INFANTRY);
    return count != 0;
  }
  COUNT_OPEN_NEIGHBOURS(count, x, y, UNIT_INFANTRY);
  return count;
}
