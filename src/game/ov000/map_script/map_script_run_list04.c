#include "types.h"
#include "game/map_script.h"
#include "game/map_unit.h"

MapScript* MapScript_Get(void);
int EventTriggers_Run(const EventTrigger* list, MapUnit* unit, int id,
                      int flag);

/* Runs the map's list 04 (see MapScript) in game state 11,
 * after the side panel slides away (Game_State11_SlidePanel); 0 when the map has none. */
int MapScript_RunList04(void) {
  MapScript* script = MapScript_Get();

  if (script != NULL && script->list04 != NULL) {
    return EventTriggers_Run(script->list04, NULL, 0, 1);
  }
  return 0;
}
