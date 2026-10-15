#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

int GetCoMoveBonus(int player, int coSlot, int powerState, int unitType);
int UnitType_GetMovement(int unitType);

/* Movement of a unit type for the current player. With a back CO, the
 * movement bonus is the average of the front and back CO's. */
int CurrentPlayer_GetUnitMoveRange(int unitType, int powerState) {
  int bonus = GetCoMoveBonus(MAP_STATE->currentPlayer, 0, powerState, unitType);
  BOOL hasBackCo =
      MAP_STATE->players[MAP_STATE->currentPlayer].back.id ? TRUE : FALSE;

  if (hasBackCo == TRUE) {
    bonus = (bonus + GetCoMoveBonus(MAP_STATE->currentPlayer, 1, powerState,
                                    unitType)) >>
            1;
  }
  return bonus + UnitType_GetMovement(unitType);
}
