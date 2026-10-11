#include "sdk/fs.h"

void IC_InvalidateRange(void *start, u32 size);
void DC_InvalidateRange(void *start, u32 size);
void MI_CpuFill8(void *dest, u8 value, u32 size);

/* Drops the cache lines over the overlay's memory and zeroes its bss. */
void Overlay_InvalidateAndZeroBss(const FSOverlayInfo *info)
{
    u8 *start;
    u32 size;
    u32 total;

    size = info->header.ram_size;
    total = size + info->header.bss_size;
    start = info->header.ram_address;

    IC_InvalidateRange(start, total);
    DC_InvalidateRange(start, total);
    MI_CpuFill8(start + size, 0, total - size);
}
