#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Unit type on tile x, y with the owner's slot base (0x00, 0x40, 0x80 or
 * 0xc0) or-ed in. UNIT_TYPE_COUNT when the tile has no unit. */
int Ov006_GetUnitCodeAt(int x, int y) {
  int result = UNIT_TYPE_COUNT;
  int slot = STATE->mapTiles->unitAt[x + STATE->mapTiles->rowStart[y]];
  int owner = slot & 0xc0;

  if (slot > 0) {
    result = STATE->units[slot].type;
    result |= owner;
  }
  return result;
}
