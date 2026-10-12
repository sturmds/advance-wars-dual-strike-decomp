#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s32 HwDiv32(s32 numerator, s32 denominator);
int CaptureIncrease(int player, int slot);
int Player_GetCaptureRate(
    u8 player); /* capture rate in percent of the player's front CO: 100 + the CO's bonus */

/* Capture points a foot unit adds to a property in one turn: shown hp
 * (1-10) times the CO's capture rate, plus the Force Rank bonus. A property
 * falls at 20. */
u8 Unit_CapturePoints(MapUnit* unit) {
  u8 shownHp;
  u8 player;
  u8 points;
  int value; /* needed to match: one temp holds the shown hp, then the rate */

  player = MAP_UNIT_OWNER(data_027c027c->unk_48[data_027c0284], unit);
  if (unit->hp == 0) {
    value = 0;
  } else {
    value = HwDiv32(unit->hp - 1, 10) + 1;
  }
  shownHp = value;
  value = Player_GetCaptureRate(player);
  points = HwDiv32(shownHp * value, 100);
  return points + CaptureIncrease(player, 0);
}
