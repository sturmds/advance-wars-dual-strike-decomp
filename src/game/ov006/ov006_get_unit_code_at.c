#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

/* Unit type on tile x, y with the owner's slot base (0x00, 0x40, 0x80 or
 * 0xc0) or-ed in. UNIT_TYPE_COUNT when the tile has no unit. */
int Ov006_GetUnitCodeAt(int x, int y) {
  int result = UNIT_TYPE_COUNT;
  int slot = MAP_STATE->mapTiles->unitAt[x + MAP_STATE->mapTiles->rowStart[y]];
  int owner = slot & 0xc0;

  if (slot > 0) {
    result = MAP_STATE->units[slot].type;
    result |= owner;
  }
  return result;
}
