#include "sdk/snd.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void SndHw_StopChannel(int channel, int hold);
void SndArm7_UpdateSharedWork(void);
void func_arm7_03801f00(int alarmNo);

void SndArm7_StopTimer(u32 channelMask, u32 captureMask, u32 alarmMask, int hold)
{
    int irqState = func_arm7_037fdb80();
    int bit;

    for (bit = 0; bit < SOUND_TIMER_SLOTS_ARM7 && alarmMask != 0; bit++, alarmMask >>= 1) {
        if (alarmMask & 1) {
            func_arm7_03801f00(bit);
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
    func_arm7_037fdb94(irqState);
    SndArm7_UpdateSharedWork();
}
