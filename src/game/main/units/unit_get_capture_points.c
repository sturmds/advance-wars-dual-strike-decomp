#include "types.h"
#include "game/Unk_02080440.h"
#include "game/math.h"

int CaptureIncrease(int player, int slot);
int Co_GetCapturePercent(
    int coId,
    int powerState); /* 100 + the CO's capture bonus in that power state; 100 with coEffectsOn off */

/* Capture points a unit adds to a property in one turn: shown hp (1-10)
 * times the capture rate of its owner's front CO with no power active,
 * plus the Force Rank bonus. */
u8 Unit_GetCapturePoints(MapUnit* unit) {
  u8 player;
  u8 shownHp;
  u8 points;
  int value; /* needed to match: one temp holds the shown hp, then the rate */

  player = MAP_UNIT_OWNER(data_027c027c->unk_48[data_027c0284], unit);
  if (unit->hp == 0) {
    value = 0;
  } else {
    value = HwDiv32(unit->hp - 1, 10) + 1;
  }
  shownHp = value;
  value = Co_GetCapturePercent(
      data_027c027c->unk_48[data_027c0284]->players[player].front.id &
          PLAYER_CO_ID_MASK,
      0);
  points = HwDiv32(shownHp * value, 100);
  return points + CaptureIncrease(player, 0);
}
