#include "types.h"
#include "game/co_data.h"

/* Repairs hp display HP (10 internal points each); returns the cost. */
int func_020cdd84(MapUnit *unit, int hp, BOOL payCost);

/* CoPowerBlock.unitBonus for Andy's Super CO Power: 5 HP back, free. */
void UnitRepair5HP(MapUnit *unit)
{
    func_020cdd84(unit, 5, FALSE);
}
