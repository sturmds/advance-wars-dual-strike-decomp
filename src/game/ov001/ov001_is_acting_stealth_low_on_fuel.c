#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_fuel_low.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* TRUE when the acting unit is a Stealth with at most half its fuel. */
BOOL Ov001_IsActingStealthLowOnFuel(void) {
  /* volatile read of the index first: the original loads it before the
     * state pointer */
  int index = *(volatile int*)&data_027c0284;
  MapUnit* unit = data_027c027c->unk_48[index]->actingUnit;

  if (unit->type != UNIT_STEALTH) {
    return FALSE;
  }
  return Unit_NeedsFuel(unit) == TRUE;
}
