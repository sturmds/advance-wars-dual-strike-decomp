#include "sdk/snd_hw.h"

void SndHw_StopChannel(int channel, int hold)
{
    SndHwChannel *regs = AUDIO_CH_REG_BLOCKS + channel;
    u32 control = AUDIO_CH_CTRL_WORD(channel << 4) & ~AUDIO_CH_PLAY_BIT;

    if (hold & 1) {
        control |= AUDIO_CH_KEEP_LAST_SAMPLE;
    }
    *(u32 *)regs = control;
}
