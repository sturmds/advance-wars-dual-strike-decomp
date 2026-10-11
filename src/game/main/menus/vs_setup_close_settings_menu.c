#include "types.h"

void Sound_PlaySe(u32 soundId);
void func_020b9890(void);
void func_020654dc(void);

void VsSetup_CloseSettingsMenu(void)
{
    Sound_PlaySe(0x10006);
    func_020b9890();
    func_020654dc();
}
