#include "types.h"
#include "game/Unk_02080440.h"

BOOL Unit_IsVisibleToActivePlayers(MapUnit* unit, s16 x, s16 y);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Unit_IsVisibleToActivePlayers for the unit in a slot of the units array,
 * at its own tile. */
BOOL UnitSlot_IsVisible(int slot) {
  MapUnit* unit = &STATE->units[slot];

  return Unit_IsVisibleToActivePlayers(unit, unit->x, unit->y);
}
