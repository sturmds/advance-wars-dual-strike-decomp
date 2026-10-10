#include "types.h"
#include "game/map_script.h"
#include "game/map_unit.h"

MapScript *func_ov000_022aea20(void);
int EventTriggers_Run(const MapScriptCommand *list, MapUnit *unit, int id, int flag);

/* Runs the map's list 18 (see MapScript); 0 when the map has none. */
int MapScript_RunList18(int flag)
{
    MapScript *script = func_ov000_022aea20();

    if (script != NULL && script->list18 != NULL) {
        return EventTriggers_Run(script->list18, NULL, 0, flag);
    }
    return 0;
}
