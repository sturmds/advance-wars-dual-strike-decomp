#include "types.h"
#include "game/map_script.h"
#include "game/map_unit.h"

MapScript* MapScript_Get(void);
int EventTriggers_Run(const EventTrigger* list, MapUnit* unit, int id,
                      int flag);

/* Runs the map's list 10 (see MapScript) for an event id and a unit (the
 * callers pass NULL or the acting unit); 0 when the map has none. */
int MapScript_RunList10(int id, MapUnit* unit) {
  MapScript* script = MapScript_Get();

  if (script != NULL && script->list10 != NULL) {
    return EventTriggers_Run(script->list10, unit, id, 1);
  }
  return 0;
}
