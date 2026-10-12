#ifndef SDK_MI_H
#define SDK_MI_H

#include "types.h"

/* One DMA channel's registers: source, destination, length and control. */
typedef struct DmaChannel {
    volatile u32 src;
    volatile u32 dest;
    volatile u32 control;
} DmaChannel;

/* The four channels start at 0x040000b0, 12 bytes apart. */
#define REG_DMA ((DmaChannel *)0x040000b0)

/* Control value for a one-unit transfer, 16-bit, fixed source and destination, starting at once. */
#define DMA_DUMMY_TRANSFER 0x81400001

/* Upper half of a channel's control register: enable, start timing and repeat. */
#define IO_DMA_CTRL_UPPER(dmaNo) (((volatile u16 *)REG_DMA)[(dmaNo) * 6 + 5])

/* Fill data registers, one word per channel; a fixed-source DMA reads them. */
#define IO_DMA_FILL_VALUE(dmaNo) (((volatile u32 *)0x040000e0)[dmaNo])

/* DMA control bits: enable, and the start timing field. */
#define DMA_ENABLE 0x80000000
#define DMA_START_MASK 0x38000000
#define DMA7_START_MASK 0x30000000   /* the ARM7's start timing field has two bits */
#define DMA_REPEAT 0x02000000
#define DMA_32BIT 0x04000000
#define DMA_IRQ 0x40000000
#define DMA_SRC_FIXED 0x01000000
#define DMA_START_VBLANK 0x08000000
#define DMA_START_HBLANK 0x10000000
#define DMA_START_DISPLAY 0x18000000
#define DMA_START_MAIN_MEMORY_DISPLAY 0x20000000
#define DMA_START_DS_CARD 0x28000000
#define DMA_START_GBA_CARD 0x30000000
#define DMA_START_GEOMETRY_FIFO 0x38000000

/* Source address adjust bit of a DMA control value: count down instead of up. */
#define DMA_SRC_DECREMENT 0x00800000

/* The top byte of an address picks the region. */
#define ADDRESS_REGION_MASK 0xff000000
#define REGION_IO 0x04000000
#define REGION_CARTRIDGE 0x08000000

/* ARM7 copies of the DMA and memory routines. Sizes are in bytes. */
void Dma_HaltChannel_Arm7(u32 dmaNo);
void Dma_SpinUntilChannelIdle_Arm7(u32 dmaNo);
void Mem_FillHalfwords_Arm7(u16 data, void *dest, u32 size);
void Mem_CopyHalfwords_Arm7(const void *src, void *dest, u32 size);
void Mem_FillWords_Arm7(u32 data, void *dest, u32 size);
void Mem_CopyWords_Arm7(const void *src, void *dest, u32 size);
void Mem_FillWordsInBlocks_Arm7(u32 data, void *dest, u32 size);
void Mem_FillBytes_Arm7(void *dest, u32 data, u32 size);
void Mem_CopyBytes_Arm7(const void *src, void *dest, u32 size);
u32 Atomic_ExchangeWord_Arm7(u32 setData, volatile u32 *destp);

#endif
