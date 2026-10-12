#include "sdk/snd_hw.h"

int func_arm7_037fe63c(void);
void func_arm7_037fe5dc(int);
int func_arm7_037fdbe4(int);
void func_arm7_03805198(int);

void SndHw_Disable(void)
{
    func_arm7_037fe63c();
    func_arm7_037fe5dc(0x80);
    func_arm7_037fdbe4(0x40000);
    func_arm7_03805198(1);
    AUDIO_SPEAKER_POWER_REG &= ~1;
}
