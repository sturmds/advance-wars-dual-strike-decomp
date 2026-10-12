#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Counts the current player's Infantry and Mech (slots 1-50), except the one
 * in skipSlot, whose unk_0c_4 bit equals flag. */
int Ai_CountFootUnitsByUnk0c4(int skipSlot, int flag) {
  int count = 0;
  int slot;
  MapUnit* unit;

  for (slot = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; slot++) {
    unit = &STATE->units[slot];
    if (unit->type != UNIT_NONE && slot != skipSlot &&
        unit->type <= UNIT_MECH) {
      if (unit->unk_0c_4 == flag) {
        count++;
      }
    }
  }
  return count;
}
