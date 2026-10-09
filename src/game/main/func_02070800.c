#include "types.h"

void Sound_PlaySe(u32 soundId);
void func_02070824(void);

void func_02070800(void)
{
    Sound_PlaySe(0x10009);
    func_02070824();
}
