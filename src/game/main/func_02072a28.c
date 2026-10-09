#include "types.h"

void MenuButtons_DisableTouch(void);
int Task_Create(void *, int);
extern u8 data_0215f8ac[];

int func_02072a28(void)
{
    MenuButtons_DisableTouch();
    return Task_Create(data_0215f8ac, 3);
}
