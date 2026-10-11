#include "types.h"

int SceneMode_Request(int a0);
void EffectScene_SetMode(int value);

void func_020f216c(void)
{
    SceneMode_Request(7);
    EffectScene_SetMode(2);
}
