#include "types.h"
#include "game/map_script.h"
#include "game/map_unit.h"

MapScript* MapScript_Get(void);
int EventTriggers_Run(const EventTrigger* list, MapUnit* unit, int id,
                      int flag);

/* Runs the map's list 0c (see MapScript) with the unit that finished its
 * action (ActingUnit_FinishAction); 0 when the map has none. */
int MapScript_RunList0c(MapUnit* unit) {
  MapScript* script = MapScript_Get();

  if (script != NULL && script->list0c != NULL) {
    return EventTriggers_Run(script->list0c, unit, 0, 1);
  }
  return 0;
}
