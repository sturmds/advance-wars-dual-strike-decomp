#include "types.h"

void Sound_PlaySe(u32 soundId);
void MenuButtons_CloseAll(void);
void func_0203579c(void);

void func_02032abc(void)
{
    Sound_PlaySe(0x10006);
    MenuButtons_CloseAll();
    func_0203579c();
}
