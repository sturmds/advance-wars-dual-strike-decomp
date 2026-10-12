#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

int GetCoMoveBonus(int player, int coSlot, int powerState, int unitType);
int UnitType_GetMovement(int unitType);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Movement of a unit type for the current player. With a back CO, the
 * movement bonus is the average of the front and back CO's. */
int CurrentPlayer_GetUnitMoveRange(int unitType, int powerState) {
  int bonus = GetCoMoveBonus(GAME->currentPlayer, 0, powerState, unitType);
  BOOL hasBackCo = GAME->players[GAME->currentPlayer].back.id ? TRUE : FALSE;

  if (hasBackCo == TRUE) {
    bonus = (bonus +
             GetCoMoveBonus(GAME->currentPlayer, 1, powerState, unitType)) >>
            1;
  }
  return bonus + UnitType_GetMovement(unitType);
}
