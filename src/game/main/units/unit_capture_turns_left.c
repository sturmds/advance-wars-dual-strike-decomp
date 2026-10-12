#include "types.h"
#include "game/map_unit.h"

s32 HwDiv32(s32 numerator, s32 denominator);
u8 Unit_CapturePoints(MapUnit* unit);

/* Capture value at which a property falls. */
#define CAPTURE_GOAL 20

/* Turns the unit needs to finish capturing, counting its progress and
 * the points it adds per turn. A unit with no shown hp gets 20. */
int Unit_CaptureTurnsLeft(MapUnit* unit) {
  int shownHp;
  int turns;
  int progress;
  int points;

  if (unit->hp == 0) {
    shownHp = 0;
  } else {
    shownHp = HwDiv32(unit->hp - 1, 10) + 1;
  }
  turns = 0;
  progress = unit->captureProgress;
  if (shownHp <= 0) {
    return CAPTURE_GOAL;
  }
  points = Unit_CapturePoints(unit);
  while (progress < CAPTURE_GOAL) {
    progress += points;
    turns++;
  }
  return turns;
}
