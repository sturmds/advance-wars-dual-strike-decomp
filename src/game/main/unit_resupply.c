#include "types.h"
#include "game/co_data.h"

/* Refill ammo and fuel to the unit type's maximum; with payCost set they
 * charge the player and stop when funds run out. Both return the cost. */
int func_020ce008(MapUnit *unit, BOOL payCost);
int func_020cdf0c(MapUnit *unit, BOOL payCost);

/* CoPowerBlock.unitBonus for Jess's powers: free ammo and fuel refill. */
void UnitResupply(MapUnit *unit)
{
    func_020ce008(unit, FALSE);
    func_020cdf0c(unit, FALSE);
}
