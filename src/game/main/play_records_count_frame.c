#include "types.h"
#include "game/play_records.h"

void Counter_IncrementCapped(u32* counter);

/* Called once per frame from the main loop: adds a frame to the play time. */
void PlayRecords_CountFrame(void) {
  u32 frames;

  frames = gPlayRecords.playFrames;
  Counter_IncrementCapped(&frames);
  gPlayRecords.playFrames = frames;
}
