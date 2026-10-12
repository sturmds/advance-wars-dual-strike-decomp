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
#define NVRAM_INSTRUCTION_READ_ID            0x9f
#define NVRAM_INSTRUCTION_RELEASE_POWER_DOWN 0xab
#define NVRAM_INSTRUCTION_DEEP_POWER_DOWN    0xb9
#define NVRAM_INSTRUCTION_CHIP_ERASE         0xc7
#define NVRAM_INSTRUCTION_SOFTWARE_RESET     0xff

/* NVRAM request ids from the ARM9 (SerialBusJob.process), one per flash
 * instruction. */
enum {
    FLASH_REQ_UNLOCK_WRITES = 0x20,
    FLASH_REQ_LOCK_WRITES = 0x21,
    FLASH_REQ_STATUS_BYTE = 0x22,
    FLASH_REQ_LOAD = 0x23,
    FLASH_REQ_LOAD_QUICK = 0x24,
    FLASH_REQ_REWRITE_PAGE = 0x25,
    FLASH_REQ_PROGRAM_PAGE = 0x26,
    FLASH_REQ_CLEAR_PAGE = 0x27,
    FLASH_REQ_CLEAR_SECTOR = 0x28,
    FLASH_REQ_SLEEP = 0x29,
    FLASH_REQ_WAKE = 0x2a,
    FLASH_REQ_CLEAR_ALL = 0x2b,
    FLASH_REQ_IDENTIFY = 0x2c,
    FLASH_REQ_RESTART = 0x2d
};

/* A request arrives as up to this many PXI words of one halfword each. */
#define NVRAM_PXI_WORDS 16
extern u16 nvramPxiWords[NVRAM_PXI_WORDS];   /* the request being collected */

/* The ARM9 buffers a request reads into or writes from must lie in main RAM. */
#define NVRAM_BUFFER_MIN 0x02000000
#define NVRAM_BUFFER_END 0x02400000

#define NVRAM_PAGE_SIZE 0x100
#define NVRAM_ADDRESS_BYTES 3

/* Bits of the flash status register. */
#define NVRAM_STATUS_BUSY         0x01   /* write or erase in progress */
#define NVRAM_STATUS_WRITE_ENABLE 0x02   /* write enable latch */
#define NVRAM_STATUS_ERROR        0x20   /* last write or erase failed; cleared by a reset */

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
void NVRAM_WriteEnable(void);
void NVRAM_WriteDisable(void);
BOOL NVRAM_IsWriteEnabled(void);   /* idle, with the write enable latch set */
BOOL NVRAM_IsIdle(void);   /* no write or erase running */
void NVRAM_SoftwareReset(void);
void NVRAM_ReadId(u8 *id);   /* the first two id bytes */

#endif
