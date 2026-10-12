#include "sdk/snd_hw.h"

int func_arm7_037fe63c(void);
void func_arm7_037fe8e4(int channel, int flag);

void SndHw_Reset(void)
{
    int channel;

    func_arm7_037fe63c();
    for (channel = 0; channel < 16; channel++) {
        func_arm7_037fe8e4(channel, 1);
    }
    AUDIO_RECORD0_CTRL = 0;
    AUDIO_RECORD1_CTRL = 0;
}
