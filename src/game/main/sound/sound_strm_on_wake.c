#include "types.h"
#include "game/sound_strm.h"

int func_0210eb0c(void);
void func_0210eb20(int irqState);
void func_0210fdc8(u32 channelMask, int arg1, u32 timerMask, int arg3);
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
        irqState = func_0210eb0c();
        SoundStrm_Refill(strm, STRM_CALLBACK_REFILL);
        func_0210eb20(irqState);
    }
    func_0210fdc8(strm->channelMask, 0, 1 << strm->timer, 0);
}
