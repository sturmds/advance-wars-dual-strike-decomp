#ifndef SDK_CTRDG_H
#define SDK_CTRDG_H

#include "types.h"

/* PXI FIFO tag of the cartridge library: the ARM9 and ARM7 exchange commands under it. */
#define FIFO_TAG_GBA_SLOT 13

/* The OSLockWord in shared main RAM that arbitrates the cartridge bus between the CPUs. */
#define GBA_SLOT_OWNER_WORD ((void *)0x027fffe8)

/* Record of one lock on the cartridge bus, filled in by Cartridge_SpinUntilSlotOwned. */
typedef struct GbaSlotLockRecord {
    u32 heldBefore;                /* nonzero when an ARM9 lock ID already held the bus */
    int savedIrqState;             /* OS_DisableInterrupts result, restored on unlock */
} GbaSlotLockRecord;

/* EXMEMCNT fields that set the GBA slot ROM access time. */
#define GBA_SLOT_INITIAL_WAIT_BITS 0xc
#define GBA_SLOT_INITIAL_WAIT_POS 2
#define GBA_SLOT_FOLLOWUP_WAIT_BIT 0x10
#define GBA_SLOT_FOLLOWUP_WAIT_POS 4

/* GBA slot ROM access time settings, as two values saved from or loaded into EXMEMCNT. */
typedef struct GbaSlotWaitStates {
    u32 romFirstAccess;            /* EXMEMCNT bits 2 to 3 */
    u32 romSecondAccess;           /* EXMEMCNT bit 4 */
} GbaSlotWaitStates;

/* Header at the start of a GBA cartridge ROM (only the fields the library reads). */
typedef struct GbaRomHeader {
    u8 pad_00[0xac];
    u32 gameCode;                  /* 0xac */
    u16 makerCode;                 /* 0xb0 */
    u8 fixedValue;                 /* 0xb2: 0x96 on real cartridges */
    u8 pad_b3[2];
    u8 reserved_b5[3];             /* 0xb5 */
    u8 pad_b8[6];
    u16 reserved_be;               /* 0xbe */
} GbaRomHeader;

#define GBA_SLOT_HEADER_PTR ((GbaRomHeader *)0x08000000)
#define GBA_HEADER_MAGIC_BYTE 0x96
/* Halfword at the end of the first 128 KiB of the ROM, checked when fixedValue is not 0x96. */
#define GBA_ROM_128K_LAST_HALFWORD ((u16 *)0x0801fffe)

/*
 * What the ARM9 remembers about the cartridge seen at start-up, in shared
 * main RAM. A cartridge that stops matching it counts as pulled out.
 */
typedef struct GbaSlotBootSnapshot {
    u16 reserved_be;               /* copy of the header word at 0xbe; 0xffff when no cartridge */
    u8 reserved_b5[3];
    u8 isAgb : 1;                  /* a GBA cartridge was seen */
    u8 pulledOut : 1;              /* the cartridge no longer matches */
    u16 makerCode;
    u32 gameCode;
} GbaSlotBootSnapshot;

#define GBA_SLOT_SNAPSHOT_PTR ((GbaSlotBootSnapshot *)0x027ffc30)

/* ARM9 side state of the library. */
typedef struct CTRDGWork {
    u16 commandDone;               /* set to 1 when the ARM7 answers an init command */
    u16 lockID;                    /* lock ID used for the cartridge bus */
} CTRDGWork;

extern CTRDGWork data_022acf14;

/* Busy-waits for roughly the given number of loop passes (Thumb code in the SDK). */
void WaitByLoop(u32 count);

void Cartridge_SendWordUntilTaken(u32 word);
void Cartridge_SpinUntilSlotOwned(int lockID, GbaSlotLockRecord *lock);
void Cartridge_ReleaseSlotIfTaken(int lockID, GbaSlotLockRecord *lock);
void Cartridge_RestoreRomWaits(GbaSlotWaitStates *timing);
void Cartridge_StashAndSlowRomWaits(GbaSlotWaitStates *saved);

/* TRUE while the cartridge seen at start-up is still in the slot; marks it pulled out otherwise. */
BOOL Cartridge_HeaderStillMatches(void);

/* TRUE when a GBA cartridge was seen at start-up (does not check it is still there). */
BOOL Cartridge_HasGbaGame(void);

/* Maker code saved from the GBA cartridge header, 0 when no GBA cartridge was seen. */
u16 Cartridge_GbaMakerCodeOrZero(void);

/* The same, 0 also when the cartridge was pulled out. */
u16 CTRDG_GetAgbMakerCode(void);

/* Game code saved from the GBA cartridge header, 0 when no GBA cartridge was seen. */
u32 Cartridge_GbaGameCodeOrZero(void);

/* The same, 0 also when the cartridge was pulled out. */
u32 CTRDG_GetAgbGameCode(void);

#endif
