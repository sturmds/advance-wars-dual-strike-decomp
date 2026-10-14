#include "types.h"
#include "game/play_records.h"

/* Word of PlayRecords at field, counted from playFrames. */
#define WORD_FROM_PLAY_FRAMES(field)                                          \
  ((int)((offsetof(PlayRecords, field) - offsetof(PlayRecords, playFrames)) / \
         sizeof(u32)))

/* Zeroes the play time and every counter after it up to unitsDestroyed.
 * The original walks them through one base pointer, playFrames. */
void PlayRecords_ResetCounters(void) {
  int i;

  for (i = 0; i < WORD_FROM_PLAY_FRAMES(unitsBought); i++) {
    (&gPlayRecords.playFrames)[i] = 0;
  }
  for (i = 0; i < (int)ARRAY_COUNT(gPlayRecords.unitsBought); i++) {
    (&gPlayRecords.playFrames)[i + WORD_FROM_PLAY_FRAMES(unitsBought)] = 0;
    (&gPlayRecords.playFrames)[i + WORD_FROM_PLAY_FRAMES(unitsDestroyed)] = 0;
  }
}
