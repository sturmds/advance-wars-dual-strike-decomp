#include "sdk/snd_hw.h"

void PMi_SetControl(int);
void func_arm7_037fe58c(int);
int func_arm7_037fdbe4(int);
int func_arm7_037fe654(void);

void SndHw_Enable(void)
{
    AUDIO_SPEAKER_POWER_REG |= 1;
    PMi_SetControl(1);
    func_arm7_037fe58c(0x100);
    func_arm7_037fdbe4(0x7AB80);
    func_arm7_037fe654();
}
