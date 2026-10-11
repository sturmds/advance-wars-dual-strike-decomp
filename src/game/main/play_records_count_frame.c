#include "types.h"
#include "game/play_records.h"

void func_020242c0(u32 *counter);

/* Called once per frame from the main loop: adds a frame to the play time. */
void PlayRecords_CountFrame(void)
{
    u32 frames;

    frames = gPlayRecords.playFrames;
    func_020242c0(&frames);
    gPlayRecords.playFrames = frames;
}
