#include "types.h"

void func_020e1a70(void);
int Task_Create(void *, int);
extern u8 data_0216d5fc[];

int func_02103310(void)
{
    func_020e1a70();
    return Task_Create(data_0216d5fc, 3);
}
