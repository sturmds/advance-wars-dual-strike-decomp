#include "types.h"

extern u8 *data_0215fed0;
void Task_Create(void *, int, void *, int);
extern u8 data_02160044[];

void func_0207bfb8(void)
{
    u8 *t0 = data_0215fed0;
    *t0 = 1;
    Task_Create(data_02160044, 3, t0, 1);
}
