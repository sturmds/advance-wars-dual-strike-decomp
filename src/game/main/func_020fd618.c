#include "types.h"

void Task_Destroy(const void *src, void *dst, u32 size);

void func_020fd618(const void *src, void *dst, u32 size)
{
    Task_Destroy(src, dst, size);
}
