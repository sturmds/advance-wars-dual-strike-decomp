#ifndef GAME_SAVE_DATA_H
#define GAME_SAVE_DATA_H

#include "types.h"

#define SAVE_SLOT_COUNT 21

/* data_02297f3c: three byte arrays with one entry per slot. */
typedef struct SaveSlotTable {
    u8 id[SAVE_SLOT_COUNT];            /* id of the record in each slot, 0xff: none (guess) */
    u8 newId[SAVE_SLOT_COUNT];         /* ids that the save buffer holds; copied to id after a full load */
    u8 flags[SAVE_SLOT_COUNT];         /* bit 4: marked by func_0201fe28 and func_0201f750, restored after */
} SaveSlotTable;

#define SAVE_SLOT_MARK 0x10

/* data_02298050: the 0x3000-byte save buffer. Only the fields that
 * Save_LoadRecord reads are known. The last 0x16 bytes hold the slot ids. */
typedef struct SaveBuffer {
    u8 pad_00[0x0c];
    u8 unk_0c;                         /* bits 4-7 set: record is skipped by Save_LoadRecord */
    u8 pad_0d;
    u16 destOffset;                    /* where the record data goes in the load destination */
    u8 pad_10[0x64 - 0x10];
    u16 dataLength;                    /* bytes of record data at data */
    u8 data[0x2fea - 0x66];
    u8 slotIds[SAVE_SLOT_COUNT];
} SaveBuffer;

#endif
