#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_fuel_low.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when one of the current player's units is a Stealth with at most
 * half its fuel. */
BOOL Ov001_HasStealthLowOnFuel(void) {
  int i;

  for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
    MapUnit* unit = &STATE->units[i];

    if (unit->type != 0 && unit->type == UNIT_STEALTH) {
      if (Unit_NeedsFuel(unit) == TRUE) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
