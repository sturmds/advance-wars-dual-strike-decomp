#include "sdk/snd.h"
#include "sdk/pxi.h"

/* Alarm handler: reports its argument, slot | tag << 8, to the ARM9, retrying until the FIFO takes it. */
void SndArm7_AlarmHandler(void *arg)
{
    while (Fifo_PushWord_Arm7(SOUND_FIFO_TAG, (u32)arg, 0) < 0) {
    }
}
