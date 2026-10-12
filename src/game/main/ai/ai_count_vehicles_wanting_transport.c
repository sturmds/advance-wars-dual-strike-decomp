#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Counts the current player's units other than Infantry and Mech (slots
 * 1-50) with transportWanted 2, which the Lander and Black Boat builders
 * read as waiting for sea transport. */
int Ai_CountVehiclesWantingTransport(void) {
  int count = 0;
  int slot;
  MapUnit* unit;

  for (slot = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; slot++) {
    unit = &STATE->units[slot];
    if (unit->type != UNIT_NONE && unit->type > UNIT_MECH) {
      if (unit->transportWanted == 2) {
        count++;
      }
    }
  }
  return count;
}
