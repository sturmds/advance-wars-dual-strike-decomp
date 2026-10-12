#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Needed to match: the unit's address as a byte offset keeps the multiply inside the loop. */
#define UNIT_AT(units, slot) \
  ((MapUnit*)((u8*)(units) + (slot) * sizeof(MapUnit)))

/* TRUE when one of the player's unit slots 1 to 50 holds a unit. */
BOOL Player_HasAnyUnit(int player) {
  int count = 1;
  int slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
  MapUnit* units = STATE->units;

  do {
    if (UNIT_AT(units, slot)->type != 0) {
      break;
    }
    count++;
    slot++;
  } while (count < 51);
  if (count == 51) {
    return FALSE;
  }
  return TRUE;
}
