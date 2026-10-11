#include "types.h"
#include "game/sound_strm.h"

int OS_DisableInterrupts(void);
void OS_RestoreInterrupts(int irqState);
void func_0210fdc8(u32 channelMask, int arg1, u32 alarmMask, int arg3);
void SoundStrm_Refill(SoundStrm *strm, int status);

/* Wake-up hook: refills the ring until the first block is next again, then
 * restarts the channels. */
void SoundStrm_OnWake(SoundStrm *strm)
{
    int irqState;

    if (!strm->flags.running) {
        return;
    }
    while (strm->blockIndex != 0) {
        irqState = OS_DisableInterrupts();
        SoundStrm_Refill(strm, STRM_CALLBACK_REFILL);
        OS_RestoreInterrupts(irqState);
    }
    func_0210fdc8(strm->channelMask, 0, 1 << strm->alarm, 0);
}
