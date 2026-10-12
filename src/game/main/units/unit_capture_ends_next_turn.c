#include "types.h"
#include "game/map_unit.h"

/* Capture points the unit adds per turn: its shown hp times a percent from
 * Co_GetCapturePercent for its front CO, plus CaptureIncrease. */
int Unit_GetCapturePoints(MapUnit* unit);

/* A property falls when its capture points reach 20. */
#define CAPTURE_POINTS_NEEDED 20

/* TRUE when the unit is capturing and its next capture turn finishes the
 * property. */
BOOL Unit_CaptureEndsNextTurn(MapUnit* unit) {
  if (unit->captureProgress == 0) {
    return FALSE;
  }
  return Unit_GetCapturePoints(unit) + unit->captureProgress >=
         CAPTURE_POINTS_NEEDED;
}
