#include "types.h"

void Player_IncUnitCount(int player);

/* Counts a unit that arrives from the other map of a two-map battle. */
void Player_CountArrivingUnit(int player) { Player_IncUnitCount(player); }
