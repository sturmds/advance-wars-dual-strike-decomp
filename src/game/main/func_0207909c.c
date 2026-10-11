#include "types.h"

void func_020790d4(void *event);
int func_02117ab4(void (*callback)(void *event), int arg);
extern int data_0229c8a0;

BOOL func_0207909c(void)
{
    return func_02117ab4(func_020790d4, data_0229c8a0) == 2;
}
