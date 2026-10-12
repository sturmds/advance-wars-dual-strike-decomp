#ifndef SDK_NVRAM_H
#define SDK_NVRAM_H

#include "types.h"
#include "sdk/spi.h"

/*
 * The firmware flash (NVRAM) on the ARM7's SPI bus. Each command selects
 * the flash, sends an instruction byte, then any address and data bytes.
 * Addresses go out as three bytes, high byte first.
 */

/* SPICNT values: enabled, 4 MHz, flash selected. HOLD keeps the flash
 * selected after the byte; the last byte of a command goes without it. */
#define NVRAM_SPICNT_HOLD 0x8900
#define NVRAM_SPICNT_LAST 0x8100

/* Flash instructions. */
#define NVRAM_INSTRUCTION_PP   0x02    /* page program: only clears bits */
#define NVRAM_INSTRUCTION_READ 0x03
#define NVRAM_INSTRUCTION_WRDI 0x04    /* write disable */
#define NVRAM_INSTRUCTION_RDSR 0x05    /* read status register */
#define NVRAM_INSTRUCTION_WREN 0x06    /* write enable */
#define NVRAM_INSTRUCTION_PW   0x0a    /* page write: erase and program */
#define NVRAM_INSTRUCTION_FAST_READ 0x0b    /* read after one dummy byte */
#define NVRAM_INSTRUCTION_SE   0xd8    /* sector erase */
#define NVRAM_INSTRUCTION_PE   0xdb    /* page erase */

#define NVRAM_PAGE_SIZE 0x100
#define NVRAM_ADDRESS_BYTES 3

static inline void NVRAM_SpiWait(void)
{
    while (REG_SPICNT & SPICNT_BUSY) {
    }
}

/* The byte that came in during the last transfer. Returning u16 keeps the
 * mask before a byte store, as the original has it. */
static inline u16 NVRAM_SpiReceived(void)
{
    return (u16)(REG_SPIDATA & 0xff);
}

void NVRAM_EraseSector(u32 address);
void NVRAM_ErasePage(u32 address);
void NVRAM_ProgramPage(u32 address, u16 size, const u8 *src);
void NVRAM_WritePage(u32 address, u16 size, const u8 *src);
void NVRAM_Read(u32 address, u32 size, u8 *dest);
void NVRAM_FastRead(u32 address, u32 size, u8 *dest);
void NVRAM_ReadStatus(u8 *status);
void NVRAM_WriteDisable(void);

#endif
