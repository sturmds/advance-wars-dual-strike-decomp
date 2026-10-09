#include "types.h"

void MenuButtons_DisableTouch(void);
int Task_Create(void *, int);
extern u8 data_0215cfa8[];

int func_020648dc(void)
{
    MenuButtons_DisableTouch();
    return Task_Create(data_0215cfa8, 3);
}
