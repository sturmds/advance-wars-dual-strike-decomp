#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

int GetCoRangeBonus(int player, int coSlot, int powerState, int unitType);
int UnitType_GetMaxRange(int unitType);

int CurrentPlayer_GetUnitMaxRange(int unitType, int powerState) {
  int bonus =
      GetCoRangeBonus(MAP_STATE->currentPlayer, 0, powerState, unitType);
  BOOL hasBackCo =
      MAP_STATE->players[MAP_STATE->currentPlayer].back.id ? TRUE : FALSE;

  if (hasBackCo == TRUE) {
    bonus = (bonus + GetCoRangeBonus(MAP_STATE->currentPlayer, 1, powerState,
                                     unitType)) >>
            1;
  }
  return bonus + UnitType_GetMaxRange(unitType);
}
