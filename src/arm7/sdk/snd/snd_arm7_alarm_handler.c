#include "sdk/snd.h"
#include "sdk/pxi.h"

/* Alarm handler: reports alarmId (slot | tag << 8) to the ARM9, retrying until the FIFO takes it. */
void SndArm7_AlarmHandler(int alarmId)
{
    while (Fifo_PushWord_Arm7(SOUND_FIFO_TAG, alarmId, 0) < 0) {
    }
}
