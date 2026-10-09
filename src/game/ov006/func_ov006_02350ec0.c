#include "types.h"

extern int data_0229eae4;
void Task_Create(void *, int, void *, int);
extern u8 data_ov006_0236a1f0[];

void func_ov006_02350ec0(void)
{
    data_0229eae4 = 1;
    Task_Create(data_ov006_0236a1f0, 3, &data_0229eae4, 1);
}
