#include "types.h"
#include "game/co_data.h"

/* Repairs hp display HP (10 internal points each); returns the cost. */
int Unit_Repair(MapUnit* unit, s16 hp, BOOL payCost);

/* CoPowerBlock.unitBonus for Andy's Super CO Power: 5 HP back, free. */
void UnitRepair5HP(MapUnit* unit) { Unit_Repair(unit, 5, FALSE); }
