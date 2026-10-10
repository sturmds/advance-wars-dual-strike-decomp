#include "types.h"
#include "game/map_script.h"
#include "game/map_unit.h"

MapScript *func_ov000_022aea20(void);
int EventTriggers_Run(const MapScriptCommand *list, MapUnit *unit, int id, int flag);

/* Runs the map's list 00 (see MapScript); 0 when the map has none. */
int MapScript_RunList00(void)
{
    MapScript *script = func_ov000_022aea20();

    if (script != NULL && script->list00 != NULL) {
        return EventTriggers_Run(script->list00, NULL, 0, 1);
    }
    return 0;
}
