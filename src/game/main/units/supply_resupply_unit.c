#include "types.h"
#include "game/map_unit.h"

int Unit_Rearm(MapUnit *unit, BOOL payCost);
int Unit_Refuel(MapUnit *unit, BOOL payCost);

/* The Supply command's refill: same body as Unit_ResupplyUnpaid, its own
 * copy in the ROM. */
int Supply_ResupplyUnit(MapUnit *unit)
{
    int total = 0;

    total += Unit_Rearm(unit, FALSE);
    total += Unit_Refuel(unit, FALSE);
    return total;
}
