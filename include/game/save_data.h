#ifndef GAME_SAVE_DATA_H
#define GAME_SAVE_DATA_H

#include "types.h"

#define SAVE_SLOT_COUNT 21

/* data_02297f3c: byte arrays with one entry per slot. */
typedef struct SaveSlotTable {
  u8 id
      [SAVE_SLOT_COUNT]; /* id of the record in each slot, 0xff: none (guess) */
  u8 newId
      [SAVE_SLOT_COUNT]; /* ids that the save buffer holds; copied to id after a full load */
  u8 flags
      [SAVE_SLOT_COUNT]; /* bit 4: marked by func_0201fe28 and Save_FindNewestRecord, restored after */
  u8 part
      [SAVE_SLOT_COUNT]; /* bits 4-7: part index of the slot in its record; bits 0-3: part count - 1 */
  u8 unk_54[SAVE_SLOT_COUNT];
} SaveSlotTable;

/* data_02297fa8: one save counter per slot (Save_FindNewestRecord takes the
 * highest), then one write count per slot. */
extern u32 data_02297fa8[2 * SAVE_SLOT_COUNT];

/* How many times a slot was written. Writes go to the free slot with the
 * lowest count. */
#define SAVE_SLOT_WRITES(slot) ((&data_02297fa8[slot])[SAVE_SLOT_COUNT])

#define SAVE_MAGIC 0x6a777364 /* "dswj" */

#define SAVE_SLOT_MARK 0x10
#define SAVE_SLOT_KEEP 0x08 /* set by Save_Format on the slots it keeps */
#define SAVE_SLOT_READ_FAILED \
  0x04 /* set by Save_ReadSlot when the check fails */
#define SAVE_SLOT_WRITE_FAILED 0x02 /* two writes in a row failed to verify */

/* data_02298050: the 0x3000-byte save buffer. Only the fields that
 * Save_LoadRecord reads are known. The last 0x16 bytes hold the slot ids. */
typedef struct SaveBuffer {
  u32 magic;   /* SAVE_MAGIC */
  u8 unk_04;   /* 0x55 or 0xaa, the opposite of the buffer's last byte */
  u8 unk_05;   /* 29 on every write */
  u8 checksum; /* byte sum of the 0x3000 bytes with these two set to 0xff and 0 */
  u8 checksumInv; /* ~checksum */
  u32 counter;    /* save counter of this slot */
  u8 part; /* which part of a record this slot holds (Save_WriteRecord); bits 4-7 set: record is skipped by Save_LoadRecord */
  u8 recordId;
  u16 destOffset; /* where the record data goes in the load destination */
  u32 slotWrites
      [SAVE_SLOT_COUNT]; /* SAVE_SLOT_WRITES of every slot when this one was written */
  u16 dataLength;        /* bytes of record data at data */
  u8 data[0x2fea - 0x66];
  u8 slotIds[SAVE_SLOT_COUNT];
} SaveBuffer;

#endif
