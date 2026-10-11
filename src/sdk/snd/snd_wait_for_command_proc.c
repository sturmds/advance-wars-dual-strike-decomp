#include "sdk/snd.h"

int func_02110554(int block);

/* Blocks until the ARM7 has finished the batch with this tag. */
void SND_WaitForCommandProc(u32 tag)
{
    if (SND_IsFinishedCommandTag(tag)) {
        return;
    }
    while (func_02110554(0) != 0) {
    }
    if (SND_IsFinishedCommandTag(tag)) {
        return;
    }
    Sound_PingArm7Driver();
    if (SND_IsFinishedCommandTag(tag)) {
        return;
    }
    do {
        func_02110554(1);
    } while (!SND_IsFinishedCommandTag(tag));
}
