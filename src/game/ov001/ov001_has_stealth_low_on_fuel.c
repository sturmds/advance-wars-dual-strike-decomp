#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_fuel_low.h"
#include "game/unit_stats.h"

/* TRUE when one of the current player's units is a Stealth with at most
 * half its fuel. */
BOOL Ov001_HasStealthLowOnFuel(void) {
  int i;

  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    MapUnit* unit = &MAP_STATE->units[i];

    if (unit->type != 0 && unit->type == UNIT_STEALTH) {
      if (Unit_NeedsFuel(unit) == TRUE) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
