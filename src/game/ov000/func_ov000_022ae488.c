#include "types.h"

void Sound_StopSubSe(int);
int func_ov000_022adbc0(int);

int func_ov000_022ae488(int a0)
{
    Sound_StopSubSe(0);
    return func_ov000_022adbc0(a0);
}
