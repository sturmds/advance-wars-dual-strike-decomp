#ifndef SDK_GX_LOAD_H
#define SDK_GX_LOAD_H

#include "types.h"

/* VramStage_Open*, VramStage_Copy* and VramStage_Close* work on a VRAM bank that is mapped to the
 * LCDC address range while the data is copied. Begin remembers the bank and
 * where it sits, Load copies at an offset inside it, End waits for DMA and
 * maps the bank back for rendering. */

#define HW_LCDC_VRAM_E \
  0x06880000 /* banks E, F, G and H as seen through LCDC while they are loaded */
#define HW_LCDC_VRAM_F 0x06890000
#define HW_LCDC_VRAM_G 0x06894000
#define LCDC_VIEW_BANK_H 0x06898000

/* DMA channel the VramStage_Copy* functions use (starts at 3); -1 copies with the CPU. */
extern int data_0216d958;

/* Sub BG extended palette bank saved by the Begin function and given back
 * by GXS_EndLoadBGExtPltt. */
extern int data_022a9894;

/* State of the main BG extended palette load (set by GX_BeginLoadBGExtPltt, the Begin function):
 * the bank mapped for loading, its address in the LCDC
 * range, and the offset the load subtracts from the palette offset it is given. */
extern int data_022a98a0;
extern int data_022a989c;
extern int data_022a9898;

/* State of the texture palette load (set by GX_BeginLoadTexPltt, the Begin function):
 * the bank mapped for loading and its address in the LCDC range. */
extern int data_022a98ac;
extern int data_022a98a8;

/* State of the texture image load (set by GX_BeginLoadTex, the Begin function):
 * the banks mapped for loading, and three values taken from the bank mask.
 * GX_LoadTex shows what they are: data_022a98a4 is the LCDC address of the
 * first block of banks, data_022a98b4 the LCDC address of a second block
 * (0 when the banks are contiguous), data_022a98b8 the size of the first
 * block in bytes. */
extern int data_022a98b0;
extern int data_022a98a4;
extern int data_022a98b4;
extern int data_022a98b8;

/* Waits until the DMA channel is idle (best guess: MI_WaitDma). */
void MI_WaitDma(int dmaNo);
/* Starts a DMA copy (best guess: MI_DmaCopy32 with callback arguments 0, 0). */
void MI_DmaCopy32Async(int dmaNo, const void* src, void* dst, u32 size,
                       void* callback, void* arg);
void MIi_CpuCopy32(const void* src, void* dst, u32 size);

/* Texture palette. */
void GX_EndLoadTexPltt(void);
void GX_LoadTexPltt(const void* src, u32 offset, u32 size);
void GX_BeginLoadTexPltt(void);

/* Texture image. */
void GX_EndLoadTex(void);
void GX_LoadTex(const void* src, u32 offset, u32 size);
void GX_BeginLoadTex(void);

/* Main engine BG extended palette. */
void GX_BeginLoadBGExtPltt(void);
void GX_EndLoadBGExtPltt(void);
void GX_LoadBGExtPltt(const void* src, u32 offset, u32 size);

/* Sub engine BG extended palette. */
void GXS_EndLoadBGExtPltt(void);
void GXS_LoadBGExtPltt(const void* src, u32 offset, u32 size);

#endif
