#include "types.h"

void Sound_PlaySe(u32 soundId);
void func_02045b44(void);

void func_020459cc(void)
{
    Sound_PlaySe(0x10009);
    func_02045b44();
}
