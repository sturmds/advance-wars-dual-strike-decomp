#include "types.h"

void Sound_StopSubSe(int);
int Sound_PlaySe(int);

int func_ov001_02351d50(void)
{
    Sound_StopSubSe(20);
    return Sound_PlaySe(165);
}
