#include "types.h"

void Sound_PlaySe(u32 soundId);
void func_020b9864(void);
void func_0205f538(void);

void func_0205d56c(void)
{
    Sound_PlaySe(0x10009);
    func_020b9864();
    func_0205f538();
}
