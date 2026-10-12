#ifndef SDK_CARD_ARM7_H
#define SDK_CARD_ARM7_H

#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/card_common.h"

/*
 * ARM7 side. Save chip access goes through the card SPI port; the ROM is read
 * through the same registers as on the ARM9.
 */
#define IO_GAMECARD_SPI_CTRL (*(volatile u16 *)0x040001a0)
#define IO_GAMECARD_SPI_BYTE (*(volatile u16 *)0x040001a2)

#define CARD_SPI_ENABLE 0x8000
#define SAVE_SPI_SELECT_PORT 0x2000           /* SPI port instead of the ROM bus */
#define CARD_SPI_HOLD 0x0040           /* keep the chip select low after this byte */
#define CARD_SPI_BUSY 0x0080

#define CARD_RESULT_TIMEOUT 4
#define CARD_RESULT_FAILURE 1          /* verify found a byte that differs */

/* Save chip commands. */
#define SAVE_CHIP_OP_FILL_ERASED_PAGE 0x02   /* write a page that was erased */
#define SAVE_CHIP_OP_FETCH_BYTES 0x03
#define SAVE_CHIP_OP_ERASE_AND_WRITE 0x0a   /* erase and write a page (flash) */
#define SAVE_CHIP_OP_WIPE_SECTOR 0xd8

/* Scanlines in one frame; used to count elapsed time from VCOUNT. */
#define GAMECARD_TIMER_FRAME_LINES 263

/* One SPI transaction: bytes left in all, and where the current transfer reads and writes. */
typedef struct CARDiSpiTransfer CARDiSpiTransfer;
struct CARDiSpiTransfer {
    int remaining;                 /* the chip select is released on the last byte */
    const u8 *src;
    u8 *dst;
    BOOL verifyOk;                 /* verify: no byte has differed so far */
};

typedef void (*CARDiSpiByteFunction)(CARDiSpiTransfer *transfer);

/* Time left in a wait counted in scanlines. */
typedef struct CARDiLineTimer {
    u16 lastLine;
    int remaining;
} CARDiLineTimer;

extern CARDiSpiTransfer data_arm7_03809860;

/*
 * The ARM7 library state, data_arm7_03809280. It has the shape of CARDiCommon
 * with the smaller ARM7 OSThread; the offsets from src on are 4 higher.
 */
typedef struct CARDiArm7Common {
    CARDiCommandArg *cmd;
    u32 command;                   /* request from the other CPU: index into the request table */
    u32 recvStep;                  /* 0: the next PXI word is a request; 1: the command block address */
    int lockOwner;                 /* OS lock id of the owner, OS_LOCK_ID_ERROR when free */
    int lockCount;
    u16 lockQueue;                 /* threads waiting for the lock, as a mask of thread ids */
    u8 pad_16[2];
    u32 lockTarget;
    u32 src;                    /* ROM read: address, destination and bytes left */
    u8 *dst;
    u32 len;
    u32 dmaNo;                     /* DMA channel for the copy to RAM, 4 or more when none */
    CARDCallback callback;
    void *callbackArg;
    void (*task)(struct CARDiArm7Common *common);   /* run by the card thread for CARD_STAT_TASK */
    volatile u32 flag;             /* GAMECARD_FLAG_*; volatile is needed to match */
    u32 priority;                  /* card thread priority */
    OSThread thread;               /* the card thread */
    OSThread *waiter;              /* thread to wake when the request ends */
    OSThreadQueue queue;           /* threads waiting for CARD_STAT_BUSY to clear */
} CARDiArm7Common;

extern CARDiArm7Common data_arm7_03809280;

/* ARM7 meaning of flag bit 0x10: a request from the other CPU waits for the card thread. */
#define GAMECARD_FLAG_PEER_JOB_PENDING 0x10

/* ROM commands the ARM7 sends. The ROM control word for a 4 byte answer has all block size bits set. */
#define GAMECARD_OP_GET_CHIP_ID 0xb8
#define GAMECARD_REPLY_LEN_4 0x07000000
#define GAMECARD_GAP_CLOCK_BITS 0x1fff        /* gap and clock fields of the control word */

/* ROM control word the card header holds for normal reads. */
#define BOOT_HEADER_CARD_TIMING (*(volatile u32 *)0x027ffe60)

/*
 * Card removal check on the ARM7. The chip ID read at boot sits at the start
 * of one of two copies of a boot info block; the halfword at 0x027ffc10 picks
 * the copy (0: the one at 0x027ff800). Bit 0 of the byte at 0x027ffc1f asks
 * for the check by chip ID instead of by the card interrupt.
 */
#define BOOT_CHIP_ID_COPY_SELECT (*(volatile u16 *)0x027ffc10)
#define BOOT_CHIP_ID_FIRST_COPY ((volatile u32 *)0x027ff800)
#define BOOT_CHIP_ID_SECOND_COPY ((volatile u32 *)0x027ffc00)
#define BOOT_REMOVAL_TEST_MODE (*(volatile u8 *)0x027ffc1f)
#define REMOVAL_TEST_USES_CHIP_ID 0x01

/* IE and IF bit: the card raised its interrupt line (it does when pulled out). */
#define IRQ_CARD_IREQ 0x100000

/* PXI word from the ARM9 under FIFO_TAG_CARD_REMOVAL: the card was removed, shut down. */
#define GAMECARD_MSG_POWER_DOWN 1
#define CARD_PXI_COMMAND_MASK 0x3f    /* command bits of a word under that tag */

/* Set once a check found the card removed; the checks return TRUE while the card is there. */
extern BOOL gCardRemoved;
BOOL Card_TestRemovalIrqFlag(void);
BOOL Card_CompareChipIdWithBoot(void);
BOOL CARD_IsPulledOut(void);

/* Sets the card thread's priority; returns the old one. */
u32 CARD_SetThreadPriority(u32 priority);

#endif
