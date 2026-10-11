#include "sdk/snd.h"

/* Blocks until the ARM7 has finished the batch with this tag. */
void SND_WaitForCommandProc(u32 tag)
{
    if (SND_IsFinishedCommandTag(tag)) {
        return;
    }
    while (SND_RecvCommandReply(0) != 0) {
    }
    if (SND_IsFinishedCommandTag(tag)) {
        return;
    }
    Sound_PingArm7Driver();
    if (SND_IsFinishedCommandTag(tag)) {
        return;
    }
    do {
        SND_RecvCommandReply(1);
    } while (!SND_IsFinishedCommandTag(tag));
}
