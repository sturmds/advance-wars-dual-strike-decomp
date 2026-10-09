#include "types.h"

void Sound_PlaySe(u32 soundId);
void func_020b9890(void);
void *VsSetup_OpenCoSelect(void);

void *func_02066624(void)
{
    Sound_PlaySe(0x10006);
    func_020b9890();
    return VsSetup_OpenCoSelect();
}
