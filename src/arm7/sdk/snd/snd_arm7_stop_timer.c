#include "sdk/snd.h"
#include "sdk/os_arm7.h"

void SndHw_StopChannel(int channel, int hold);
void SndArm7_UpdateSharedWork(void);
void SndArm7_StopAlarm(int alarmNo);

void SndArm7_StopTimer(u32 channelMask, u32 captureMask, u32 alarmMask, int hold)
{
    int irqState = Cpu_MaskIrq_Arm7();
    int bit;

    for (bit = 0; bit < SOUND_TIMER_SLOTS_ARM7 && alarmMask != 0; bit++, alarmMask >>= 1) {
        if (alarmMask & 1) {
            SndArm7_StopAlarm(bit);
        }
    }
    for (bit = 0; bit < 16 && channelMask != 0; bit++, channelMask >>= 1) {
        if (channelMask & 1) {
            SndHw_StopChannel(bit, hold);
        }
    }
    if (captureMask & 1) {
        AUDIO_RECORD0_CNT = 0;
    }
    if (captureMask & 2) {
        AUDIO_RECORD1_CNT = 0;
    }
    Cpu_RestoreIrqBit_Arm7(irqState);
    SndArm7_UpdateSharedWork();
}
