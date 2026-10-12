#include "types.h"

void Player_DecUnitCount(int player);

/* UnitAction_Join's count update: the unit type is passed but not used. */
void func_020c73b4(int player, int unitType) { Player_DecUnitCount(player); }
