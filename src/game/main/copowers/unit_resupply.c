#include "types.h"
#include "game/co_data.h"

/* Refill ammo and fuel to the unit type's maximum; with payCost set they
 * charge the player and stop when funds run out. Both return the cost. */
int Unit_Rearm(MapUnit* unit, BOOL payCost);
int Unit_Refuel(MapUnit* unit, BOOL payCost);

/* CoPowerBlock.unitBonus for Jess's powers: free ammo and fuel refill. */
void UnitResupply(MapUnit* unit) {
  Unit_Rearm(unit, FALSE);
  Unit_Refuel(unit, FALSE);
}
