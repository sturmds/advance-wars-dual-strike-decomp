#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s32 HwDiv32(s32 numerator, s32 denominator);
int Player_GetFuelRate(int player);
int Player_GetAirFuelChange(int player, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Needed to match: at these two points the original reads data_027c0284 and
 * then data_027c027c again instead of reusing the state pointer it already
 * holds. Volatile reads, index first, are the only form found that does this. */
#define RELOAD_STATE(dst)                                             \
  {                                                                   \
    int index_ = *(volatile int*)&data_027c0284;                      \
    dst = (*(Unk_02080440* volatile*)&data_027c027c)->unk_48[index_]; \
  }

/* Daily fuel use added on top of Player_GetAirFuelChange's value, per turn spent
 * travelling. */
#define FUEL_PER_DAY_STEALTH 8
#define FUEL_PER_DAY_AIR_SUB 5
#define FUEL_PER_DAY_OTHER 1

/* TRUE when the acting unit can reach (x, y) with fuel left over for the
 * extra days. rangeMap holds the move cost there; the days are the whole
 * turns beyond the first at movesPerTurn. Ground units and unreached tiles
 * (cost 0) always pass. */
BOOL AiUnit_HasFuelForTile(int x, int y, int movesPerTurn) {
  int days;
  int unitType;
  Unk_02080440_48* state;

  if (!(Ov000_gUnitStats[STATE->actingUnit->type].moveGroup &
        (MOVE_GROUP_AIR | MOVE_GROUP_SEA))) {
    return TRUE;
  }
  if (STATE->rangeMap[y][x] == 0) {
    return TRUE;
  }
  days = HwDiv32(STATE->rangeMap[y][x] - 1, movesPerTurn);
  RELOAD_STATE(state);
  unitType = state->actingUnit->type;
  if (unitType == UNIT_STEALTH) {
    if (state->actingUnit->fuel -
            Player_GetFuelRate(state->currentPlayer) * state->rangeMap[y][x] <
        days * (Player_GetAirFuelChange(state->currentPlayer, unitType) +
                FUEL_PER_DAY_STEALTH)) {
      return FALSE;
    }
  } else if ((Ov000_gUnitStats[unitType].moveGroup & MOVE_GROUP_AIR) ||
             unitType == UNIT_SUB) {
    RELOAD_STATE(state);
    if (state->actingUnit->fuel -
            Player_GetFuelRate(state->currentPlayer) * state->rangeMap[y][x] <
        days * (Player_GetAirFuelChange(state->currentPlayer, unitType) +
                FUEL_PER_DAY_AIR_SUB)) {
      return FALSE;
    }
  } else {
    if (state->actingUnit->fuel -
            Player_GetFuelRate(state->currentPlayer) * state->rangeMap[y][x] <
        days * (Player_GetAirFuelChange(state->currentPlayer, unitType) +
                FUEL_PER_DAY_OTHER)) {
      return FALSE;
    }
  }
  return TRUE;
}
