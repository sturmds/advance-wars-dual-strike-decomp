#include "types.h"

void MenuButtons_DisableTouch(void);
int Task_Create(void *, int);
extern u8 data_0216a52c[];

int func_020fc234(void)
{
    MenuButtons_DisableTouch();
    return Task_Create(data_0216a52c, 3);
}
