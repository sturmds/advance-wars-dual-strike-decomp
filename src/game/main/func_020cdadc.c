#include "types.h"
#include "game/map_unit.h"

int Unit_Rearm(MapUnit *unit, BOOL payCost);
int Unit_Refuel(MapUnit *unit, BOOL payCost);

int func_020cdadc(MapUnit *unit)
{
    int total = 0;

    total += Unit_Rearm(unit, FALSE);
    total += Unit_Refuel(unit, FALSE);
    return total;
}
