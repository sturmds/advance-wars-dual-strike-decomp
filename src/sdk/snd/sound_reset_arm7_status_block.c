#include "sdk/snd.h"
#include "sdk/os.h"

void Sound_ResetArm7StatusBlock(SndSharedWork *work)
{
    int i;
    int j;

    work->playerStatus = 0;
    work->channelStatus = 0;
    work->captureStatus = 0;
    work->finishCommandTag = 0;
    for (i = 0; i < 16; i++) {
        work->player[i].tickCounter = 0;
        for (j = 0; j < 16; j++) {
            work->player[i].variable[j] = -1;
        }
    }
    for (i = 0; i < 16; i++) {
        work->globalVariable[i] = -1;
    }
    DC_FlushRange(work, sizeof(SndSharedWork));
}
