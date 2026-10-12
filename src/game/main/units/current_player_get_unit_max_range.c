#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

int GetCoRangeBonus(int player, int coSlot, int powerState, int unitType);
int UnitType_GetMaxRange(int unitType);
extern Unk_02080440* data_027c027c;
extern int data_027c0284;
#define GAME (data_027c027c->unk_48[data_027c0284])

int CurrentPlayer_GetUnitMaxRange(int unitType, int powerState) {
  int bonus = GetCoRangeBonus(GAME->currentPlayer, 0, powerState, unitType);
  BOOL hasBackCo = GAME->players[GAME->currentPlayer].back.id ? TRUE : FALSE;

  if (hasBackCo == TRUE) {
    bonus = (bonus +
             GetCoRangeBonus(GAME->currentPlayer, 1, powerState, unitType)) >>
            1;
  }
  return bonus + UnitType_GetMaxRange(unitType);
}
