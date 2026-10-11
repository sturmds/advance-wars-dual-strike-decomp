#ifndef SDK_CARD_H
#define SDK_CARD_H

#include "types.h"
#include "sdk/os.h"

/* Game card registers. */
#define IO_GAMECARD_MASTER (*(volatile u8 *)0x040001a1)
#define IO_GAMECARD_ROM_CTRL (*(volatile u32 *)0x040001a4)
#define IO_GAMECARD_CMD_BYTES ((volatile u8 *)0x040001a8)   /* 8 command bytes, sent first byte first */
#define IO_GAMECARD_READ_PORT (*(volatile u32 *)0x04100010)

/* MASTERCNT: card enable, with interrupts off and ROM (not backup) mode. */
#define CARDMST_ENABLE 0x80
#define CARDMST_IRQ 0x40

/* GAMECARD_ROM_CTRL bits. */
#define CARD_DATA_READY 0x00800000
#define CARD_COMMAND_MASK 0x07000000   /* block size field */
#define GAMECARD_XFER_ONE_PAGE 0x01000000         /* one 512-byte block */
#define CARD_RESET_HI 0x20000000
#define CARD_START 0x80000000

/* Card command: read one 512-byte page. */
#define CARD_CMD_READ_PAGE 0xb7

#define GAMECARD_ROM_PAGE_BYTES 512

/*
 * Backup memory (save chip) support. A backup type packs the chip kind in
 * bits 0-7 and log2 of its size in bytes in bits 8-15.
 */
#define SAVE_CHIP_TYPE_EEPROM 1
#define SAVE_CHIP_TYPE_FLASH 2
#define SAVE_CHIP_TYPE_FRAM 3

/* Chip geometry and timings that CardSave_FillGeometryFromType fills from the type. */
typedef struct CARDiBackupSpec {
    u32 totalSize;
    u32 sectorSize;
    u32 pageSize;
    u32 addrWidth;          /* bytes in a chip address */
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1C;
    u32 unk_20;
    u32 unk_24;
} CARDiBackupSpec;

/* What the card thread sends to the other CPU for one backup request. */
typedef struct CARDiCommandArg {
    int result;                    /* GAMECARD_STATUS_*; 0 means success */
    u32 type;                      /* backup type */
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
    u32 unk_14;
    CARDiBackupSpec spec;
} CARDiCommandArg;

#define CARD_RESULT_SUCCESS 0
#define CARD_RESULT_UNSUPPORTED 3

/* Lock targets of CARDi_LockResource and CARDi_UnlockResource. */
#define CARD_TARGET_ROM 1
#define CARD_TARGET_BACKUP 2

/* CARDiCommon.flag bits. */
#define CARD_STAT_INIT 0x01            /* CARD_Init has run */
#define GAMECARD_FLAG_PEER_GREETED 0x02        /* the other CPU has been sent its first request */
#define CARD_STAT_BUSY 0x04            /* a request is running */
#define CARD_STAT_TASK 0x08            /* the card thread has a task to run */
#define GAMECARD_FLAG_WORKER_SLEEPS 0x10     /* the card thread sleeps until the request ends */
#define GAMECARD_FLAG_AWAIT_PEER_REPLY 0x20        /* waiting for the other CPU's answer */

/* CARDiCommon.priorityMode: base priority, or follow the requester's. */
#define GAMECARD_PRIO_OUTRANK_CALLER 32
#define GAMECARD_PRIO_YIELD_TO_CALLER 33

#define GAMECARD_WORKER_STACK_BYTES 0x400
#define GAMECARD_BOUNCE_BUF_BYTES 0x100

typedef struct CARDiCommon CARDiCommon;
typedef void (*GameCardTaskStep)(CARDiCommon *common);
typedef void (*CARDCallback)(void *arg);

/* CARD library state, data_022abb00 (0x5e0 bytes with the thread stack). */
struct CARDiCommon {
    CARDiCommandArg *cmd;
    u32 unk_04;
    volatile int lockOwner;        /* OS lock id of the owner, OS_LOCK_ID_ERROR when free; volatile is needed to match */
    int lockCount;
    OSThreadQueue lockQueue;
    u32 lockTarget;
    u32 src;
    u32 dst;
    u32 len;
    u32 unk_24;
    CARDCallback callback;
    void *callbackArg;
    GameCardTaskStep task;
    u32 flag;
    u32 priorityMode;              /* thread priority, or GAMECARD_PRIO_* */
    OSThread thread;
    OSThread *waiter;              /* thread to wake when the request ends */
    OSThreadQueue queue;           /* threads waiting for CARD_STAT_BUSY to clear */
    u8 unk_D8[8];
    u8 pageBuffer[GAMECARD_BOUNCE_BUF_BYTES];
    u8 stack[GAMECARD_WORKER_STACK_BYTES];
};

extern CARDiCommon data_022abb00;

/* PXI messages. Tag 11 carries backup requests; tag 14 the card removal notice. */
#define FIFO_TAG_CARD_SERVICE 11
#define FIFO_TAG_CARD_REMOVAL 14
#define GAMECARD_MSG_REMOVED 17
#define GAMECARD_REQ_HANDSHAKE 0
#define GAMECARD_STATUS_RESEND 4       /* the other CPU asks for the request to be sent again */

/* Set once the other CPU reports the card removed, and the game's handler for that. */
extern BOOL data_022ac320;
extern BOOL (*data_022ac324)(void);

/* Status halfword set by the other CPU: bit 15 asks for a power off after card removal. */
#define REMOVAL_HALT_STATUS_WORD (*(volatile u16 *)0x027fffa8)
#define REMOVAL_HALT_POWER_BIT 0x8000

/* IRQ mask bit: a card data transfer ended. */
#define IRQ_CARD_DATA 0x80000

/* ROM read work, data_022ac100: the page being read and its control word. */
typedef struct CARDiRomPage CARDiRomPage;
struct CARDiRomPage {
    BOOL (*task)(CARDiRomPage *page);   /* runs after each page, TRUE while more is left */
    u32 control;                        /* IO_GAMECARD_ROM_CTRL value that starts a page read */
    u32 address;                        /* ROM address of the page held in data */
    u8 unk_0C[20];
    u8 data[GAMECARD_ROM_PAGE_BYTES];
};
extern CARDiRomPage data_022ac100;

#endif
