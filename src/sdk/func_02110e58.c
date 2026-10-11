#include "types.h"

void PXI_InitFifo(const void *src, void *dst, u32 size);

void func_02110e58(const void *src, void *dst, u32 size)
{
    PXI_InitFifo(src, dst, size);
}
