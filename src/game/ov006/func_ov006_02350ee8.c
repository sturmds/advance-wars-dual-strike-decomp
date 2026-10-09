#include "types.h"

void Sound_PlaySe(int);
int func_ov006_02350f20(int);

int func_ov006_02350ee8(void)
{
    Sound_PlaySe(0x10009);
    return func_ov006_02350f20(1);
}
