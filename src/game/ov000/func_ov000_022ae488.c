#include "types.h"

void Sound_StopSubSe(int);
int ShowEntry_Finish(int);

int func_ov000_022ae488(int a0)
{
    Sound_StopSubSe(0);
    return ShowEntry_Finish(a0);
}
