#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s32 HwDiv32(s32 numerator, s32 denominator);
int Player_GetFuelRate(int player);
int GetUnitMovement(int player, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Movement points the unit can spend this turn: its movement, or less when
 * its fuel runs out first. */
int Unit_GetUsableMovement(MapUnit* unit) {
  int move = HwDiv32(unit->fuel, Player_GetFuelRate(STATE->currentPlayer));

  if (GetUnitMovement(STATE->currentPlayer, unit->type) <= move) {
    move = GetUnitMovement(STATE->currentPlayer, unit->type);
  }
  return move;
}
