#include "sdk/snd.h"

int func_arm7_037fe1cc(u32 tag, u32 data, int err); /* PXI_SendWordByFifo: negative while the FIFO is full */

/* Alarm handler: reports alarmId (slot | tag << 8) to the ARM9, retrying until the FIFO takes it. */
void SndArm7_AlarmHandler(int alarmId)
{
    while (func_arm7_037fe1cc(SOUND_FIFO_TAG, alarmId, 0) < 0) {
    }
}
