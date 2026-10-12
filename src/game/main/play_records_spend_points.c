#include "types.h"
#include "game/play_records.h"

/* Takes points from gPlayRecords.points (Hachi's shop) and returns TRUE, or
 * returns FALSE when there are not enough. */
BOOL PlayRecords_SpendPoints(u32 points) {
  u32 have = gPlayRecords.points;
  if (have < points) {
    return FALSE;
  }
  gPlayRecords.points = have - points;
  return TRUE;
}
