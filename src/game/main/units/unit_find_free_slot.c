#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Returns the first unused unit slot (type 0) of a player, or NULL if the
 * player's 50 slots are all in use. */
MapUnit* Unit_FindFreeSlot(int player) {
  int slot;

  for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
    if (STATE->units[slot].type == 0) {
      return &STATE->units[slot];
    }
  }
  return NULL;
}
