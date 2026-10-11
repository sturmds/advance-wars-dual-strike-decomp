#include "types.h"
#include "sdk/gx_load.h"

void GX_LoadBGExtPltt(const void *src, u32 offset, u32 size)
{
    void *dst = (void *)(data_022a989c + offset - data_022a9898);

    if (data_0216d958 != -1) {
        func_0210f0c8(data_0216d958, src, dst, size, NULL, NULL);
    } else {
        MIi_CpuCopy32(src, dst, size);
    }
}
