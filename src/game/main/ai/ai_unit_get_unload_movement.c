#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int AiActingUnit_GetSearchMovement(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define SEARCH_RANGE_MAX 110

/* Three quarters of the search movement AiActingUnit_GetSearchMovement
 * gives for the unit, which is made the acting unit for the call. An empty
 * slot (type 0) gets the maximum. */
int AiUnit_GetUnloadMovement(MapUnit* unit) {
  MapUnit* previous = STATE->actingUnit;
  int movement;

  if (unit->type == UNIT_NONE) {
    return SEARCH_RANGE_MAX;
  }
  STATE->actingUnit = unit;
  movement = AiActingUnit_GetSearchMovement();
  STATE->actingUnit = previous;
  return movement * 3 >> 2;
}
