#include "types.h"
#include "game/Unk_02080440.h"

int UnitType_GetVision(int unitType);
int VisionIncrease(int player);

/* Vision range of a unit type for a player: the type's vision plus the
 * player's CO bonus, 1 less in rain, at least 1. */
int Player_GetUnitVision(int playerIndex, int unitType) {
  int base = UnitType_GetVision(unitType);
  int bonus = VisionIncrease(playerIndex);
  int raining =
      data_027c027c->unk_48[data_027c0284]->unk_1c4->weather == WEATHER_RAIN;
  int vision = base + bonus + (raining ? -1 : 0);

  if (vision <= 1) {
    vision = 1;
  }
  return vision;
}
