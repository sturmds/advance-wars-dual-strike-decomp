#ifndef GAME_UNIT_FUEL_LOW_H
#define GAME_UNIT_FUEL_LOW_H

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

/* TRUE when the unit has at most half of its type's full fuel. Types with
 * no fuel never count. */
static inline BOOL Unit_IsFuelAtMostHalf(MapUnit* unit) {
  int maxFuel = Ov000_gUnitStats[unit->type].fuel;
  BOOL low = FALSE;

  if (maxFuel != 0 && unit->fuel <= maxFuel >> 1) {
    low = TRUE;
  }
  return low;
}

/* The original turns the result into a BOOL twice more before the
 * comparison with TRUE; these two wrappers keep both steps. */
static inline BOOL Unit_IsFuelLow(MapUnit* unit) {
  BOOL low = FALSE;

  if (Unit_IsFuelAtMostHalf(unit)) {
    low = TRUE;
  }
  return low;
}

static inline BOOL Unit_NeedsFuel(MapUnit* unit) {
  return Unit_IsFuelLow(unit) ? TRUE : FALSE;
}

#endif
