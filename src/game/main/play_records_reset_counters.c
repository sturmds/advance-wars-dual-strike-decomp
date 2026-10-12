#include "types.h"
#include "game/play_records.h"

/* Zeroes the play time and every counter after it up to unitsDestroyed.
 * Words 50-76 from playFrames are unitsBought, 77-103 are unitsDestroyed;
 * the original walks them through one base pointer. */
void PlayRecords_ResetCounters(void) {
  int i;

  for (i = 0; i < 50; i++) {
    (&gPlayRecords.playFrames)[i] = 0;
  }
  for (i = 0; i < 27; i++) {
    (&gPlayRecords.playFrames)[i + 50] = 0;
    (&gPlayRecords.playFrames)[i + 77] = 0;
  }
}
