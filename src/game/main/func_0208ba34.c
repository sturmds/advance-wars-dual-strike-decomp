#include "types.h"

void MenuButtons_DisableTouch(void);
int Task_Create(void *, int);
extern u8 data_0216179c[];

int func_0208ba34(void)
{
    MenuButtons_DisableTouch();
    return Task_Create(data_0216179c, 3);
}
