#include "types.h"

void Sound_PlaySe(int);
int Ov006_PostRequest(int);

int func_ov006_02350ee8(void)
{
    Sound_PlaySe(0x10009);
    return Ov006_PostRequest(1);
}
