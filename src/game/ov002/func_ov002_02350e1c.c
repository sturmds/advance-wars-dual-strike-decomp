#include "types.h"

void Task_Create(void *, int);
extern u8 data_ov002_02360044[];

void func_ov002_02350e1c(void)
{
    Task_Create(data_ov002_02360044, 1);
}
