#include "types.h"

int Sound_PlaySe(int);
int func_02048294(int);

int func_02048270(void)
{
    int t0 = Sound_PlaySe(0x10006);
    return func_02048294(t0);
}
