#include "types.h"

void Sound_PlaySe(int);
int func_02047830(int);

int func_02046e18(u8 *a0)
{
    Sound_PlaySe(0x10002);
    return func_02047830(*a0);
}
