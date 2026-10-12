#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

int Player_GetFuelRate(
    int player); /* fuel per movement point: 2 in snow unless the front CO is Olaf (checked only with coEffectsOn set) */

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Fuel a unit burns for a move costing moveCost (movement points; a
 * guess from the callers), times the per-point rate, at most the fuel it has.
 * Oozium uses none. */
int Unit_GetFuelUsed(MapUnit* unit, int moveCost) {
  Unk_02080440_48* state = data_027c027c->unk_48[data_027c0284];
  int player = MAP_UNIT_OWNER(state, unit);
  int fuel;

  if (unit->type == UNIT_OOZIUM) {
    return 0;
  }
  fuel = Player_GetFuelRate(player) * moveCost;
  if (unit->fuel < fuel) {
    fuel = unit->fuel;
  }
  return fuel;
}
