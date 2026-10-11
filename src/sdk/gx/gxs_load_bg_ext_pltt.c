#include "types.h"
#include "sdk/gx_load.h"

void GXS_LoadBGExtPltt(const void *src, u32 offset, u32 size)
{
    if (data_0216d958 != -1) {
        MI_DmaCopy32Async(data_0216d958, src, (void *)(LCDC_VIEW_BANK_H + offset), size, NULL, NULL);
    } else {
        MIi_CpuCopy32(src, (void *)(LCDC_VIEW_BANK_H + offset), size);
    }
}
