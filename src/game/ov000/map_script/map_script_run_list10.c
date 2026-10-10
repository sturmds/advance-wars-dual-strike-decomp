#include "types.h"
#include "game/map_script.h"
#include "game/map_unit.h"

MapScript *func_ov000_022aea20(void);
int EventTriggers_Run(const MapScriptCommand *list, MapUnit *unit, int id, int flag);

/* Runs the map's list 10 (see MapScript) for an event id and a unit (the
 * callers pass NULL or the acting unit); 0 when the map has none. */
int MapScript_RunList10(int id, MapUnit *unit)
{
    MapScript *script = func_ov000_022aea20();

    if (script != NULL && script->list10 != NULL) {
        return EventTriggers_Run(script->list10, unit, id, 1);
    }
    return 0;
}
