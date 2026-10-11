#include "types.h"
#include "sdk/gx.h"

/* DMA channel used for GX memory transfers; -1 means use the CPU. */
extern int data_0216d958;

/* SDK DMA fills (async, with callback; sync) and CPU 32-bit fill. */
void MI_DmaFill32Async(int dmaNo, void *dest, u32 data, u32 size, void *callback, void *arg);
void MI_DmaFill32(int dmaNo, void *dest, u32 data, u32 size);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

/* Zeroes the edge color table, the fog and toon tables, and the shininess
 * table. */
void G3X_InitTable(void)
{
    int i;

    if (data_0216d958 != -1) {
        MI_DmaFill32Async(data_0216d958, (void *)REG_EDGE_COLOR, 0, 16, NULL, NULL);
        MI_DmaFill32(data_0216d958, (void *)REG_FOG_TABLE, 0, 96);
    } else {
        volatile u32 zeroEdge = 0;
        volatile u32 zeroFog;

        MIi_CpuClear32(zeroEdge, (void *)REG_EDGE_COLOR, 16);
        zeroFog = 0;
        MIi_CpuClear32(zeroFog, (void *)REG_FOG_TABLE, 96);
    }
    for (i = 0; i < 32; i++) {
        *REG_SHININESS = 0;
    }
}
