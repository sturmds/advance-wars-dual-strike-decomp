#include "types.h"

void SoundArc_ForgetLoadedItem(int, int, int);
int func_02110c90(int);

int func_02106f94(int a0, int a1, int a2, int a3)
{
    SoundArc_ForgetLoadedItem(a0, a2, a3);
    return func_02110c90(a0);
}
