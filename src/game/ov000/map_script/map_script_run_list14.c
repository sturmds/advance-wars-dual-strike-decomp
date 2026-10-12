#include "types.h"
#include "game/map_script.h"
#include "game/map_unit.h"

MapScript* MapScript_Get(void);
int EventTriggers_Run(const EventTrigger* list, MapUnit* unit, int id,
                      int flag);

/* Runs the map's list 14 (see MapScript); 0 when the map has none. */
int MapScript_RunList14(void) {
  MapScript* script = MapScript_Get();

  if (script != NULL && script->list14 != NULL) {
    return EventTriggers_Run(script->list14, NULL, 0, 1);
  }
  return 0;
}
