#ifndef SDK_GX_LOAD_H
#define SDK_GX_LOAD_H

#include "types.h"

/* VramStage_Open*, VramStage_Copy* and VramStage_Close* work on a VRAM bank that is mapped to the
 * LCDC address range while the data is copied. Begin remembers the bank and
 * where it sits, Load copies at an offset inside it, End waits for DMA and
 * maps the bank back for rendering. */

#define LCDC_VIEW_BANK_H 0x06898000 /* bank H as seen through LCDC while it is loaded */

/* DMA channel the VramStage_Copy* functions use (starts at 3); -1 copies with the CPU. */
extern int data_0216d958;

/* Waits until the DMA channel is idle (best guess: MI_WaitDma). */
void func_0210f034(int dmaNo);
/* Starts a DMA copy (best guess: MI_DmaCopy32 with callback arguments 0, 0). */
void func_0210f0c8(int dmaNo, const void *src, void *dst, u32 size, void *callback, void *arg);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);

/* Sub engine BG extended palette. */
void GXS_EndLoadBGExtPltt(void);
void GXS_LoadBGExtPltt(const void *src, u32 offset, u32 size);

#endif
