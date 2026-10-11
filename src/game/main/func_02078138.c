#include "types.h"

void WM_GetSharedDataAddress(int, int, int, void *);
extern int data_0229c8ac;
extern int data_0229c8b0;

void func_02078138(int a0)
{
    WM_GetSharedDataAddress(data_0229c8ac, data_0229c8b0, a0, &data_0229c8ac);
}
