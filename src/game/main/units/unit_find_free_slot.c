#include "types.h"
#include "game/Unk_02080440.h"

/* Returns the first unused unit slot (type 0) of a player, or NULL if the
 * player's 50 slots are all in use. */
MapUnit* Unit_FindFreeSlot(int player) {
  int slot;

  for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
    if (MAP_STATE->units[slot].type == 0) {
      return &MAP_STATE->units[slot];
    }
  }
  return NULL;
}
