#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Counts the current player's units of the given type (slots 1-50) that
 * carry nothing in their first cargo slot. */
int Ai_CountEmptyTransports(int type) {
  int count = 0;
  int slot;
  MapUnit* unit;

  for (slot = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; slot++) {
    unit = &STATE->units[slot];
    if (unit->type != UNIT_NONE && unit->type == type) {
      if (unit->cargo[0] == 0) {
        count++;
      }
    }
  }
  return count;
}
