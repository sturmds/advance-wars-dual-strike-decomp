#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s32 HwDiv32(s32 numerator, s32 denominator);
int Player_GetFuelRate(int player);
int GetUnitMovement(int player, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Movement points the unit can spend this turn: its movement, or less when
 * its fuel runs out first. Like Unit_GetUsableMovement, for the unit's owner
 * instead of the current player. */
int Unit_GetMovePoints(MapUnit* unit) {
  int fuel = unit->fuel;
  int movement = GetUnitMovement(MAP_UNIT_OWNER(STATE, unit), unit->type);
  int move = HwDiv32(fuel, Player_GetFuelRate(MAP_UNIT_OWNER(STATE, unit)));

  if (move >= movement) {
    move = movement;
  }
  return move;
}
