#include "types.h"

void CoSelect_ResetToUnlockedCos(void);
int Task_Create(void *, int);
extern u8 data_0216d5fc[];

int func_02103310(void)
{
    CoSelect_ResetToUnlockedCos();
    return Task_Create(data_0216d5fc, 3);
}
