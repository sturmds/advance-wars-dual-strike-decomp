#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

/* Whether the acting unit is a Carrier that has already waited. Name from the code; no caller found. */
BOOL ActingUnit_IsWaitedCarrier(void) {
  MapUnit* unit = data_027c027c->unk_48[data_027c0284]->actingUnit;

  if (unit->type != UNIT_CARRIER) {
    return FALSE;
  }
  return (unit->flags & MAP_UNIT_WAITED) != 0;
}
