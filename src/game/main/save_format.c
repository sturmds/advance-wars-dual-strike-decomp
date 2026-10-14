#include "types.h"
#include "game/save_data.h"

/* also declared in src/game/main/save_scan_slots.c and 1 more */
int gNextSaveCounter;
void (*gSaveResetFunc)(void); /* also declared in src/game/main/save_init.c */
/* also declared in src/game/main/save_delete_record.c and 4 more */
u8* gSaveBuffer;
/* also declared in src/game/main/save_delete_record.c and 2 more */
int (*gSavePackFunc)(u8* dest);
int data_02297f30;
extern SaveSlotTable data_02297f3c;
extern u8 data_0229a050[];

u16 Save_FindNewestRecord(int recordId);
int Save_ReadSlot(u16 slot);
void Save_WriteRecord(int recordId, u8* src, int size);
u16 Save_NoBackupMemory(u16 slot);

/* Resets the save to its base record 0, the copy with save counter 0
 * (written from gSaveBuffer when it is missing). The other copies of
 * record 0 are erased, older ones first, and every other slot is freed (id
 * 0xff). Erasing a slot: up to four tries of Save_NoBackupMemory (which
 * only reads a flag) and then a read, until the read fails; the slot is then
 * marked bad (flag bit 2) with counter 0. The erase reading is a guess. */
void Save_Format(void) {
  int size;
  int i;
  int try
    ;

  gNextSaveCounter = 0;
  gSaveResetFunc();
  size = gSavePackFunc(gSaveBuffer);
  for (i = 0; i < SAVE_SLOT_COUNT; i++) {
    data_02297f3c.flags[i] &= ~SAVE_SLOT_KEEP;
  }
  if (Save_FindNewestRecord(0xff) == 0xffff) {
    Save_WriteRecord(0, gSaveBuffer, size);
  } else {
    for (i = 0; i < SAVE_SLOT_COUNT; i++) {
      if (data_02297f3c.flags[i] & SAVE_SLOT_MARK) {
        data_02297f3c.flags[i] |= SAVE_SLOT_KEEP;
        data_02297f3c.flags[i] &= ~SAVE_SLOT_MARK;
      }
    }
  }
  Save_FindNewestRecord(0);

  for (i = 0; i < SAVE_SLOT_COUNT; i++) {
    if (!(data_02297f3c.flags[i] & (SAVE_SLOT_MARK | SAVE_SLOT_KEEP)) &&
        data_02297f3c.id[i] == 0) {
      for (try = 0; try < SAVE_WRITE_TRIES; try ++) {
        Save_NoBackupMemory(i);
        if (Save_ReadSlot(i)) {
          data_02297f3c.unk_54[i] = data_0229a050[0xfff];
          data_02297f3c.newId[i] = 0xff;
          data_02297f3c.id[i] = data_02297f3c.newId[i];
          data_02297f3c.flags[i] |= 4;
          data_02297fa8[i] = 0;
          break;
        }
      }
    }
  }
  for (i = 0; i < SAVE_SLOT_COUNT; i++) {
    if ((data_02297f3c.flags[i] & SAVE_SLOT_MARK) &&
        !(data_02297f3c.flags[i] & SAVE_SLOT_KEEP) &&
        data_02297f3c.id[i] == 0) {
      for (try = 0; try < SAVE_WRITE_TRIES; try ++) {
        Save_NoBackupMemory(i);
        if (Save_ReadSlot(i)) {
          data_02297f3c.unk_54[i] = data_0229a050[0xfff];
          data_02297f3c.newId[i] = 0xff;
          data_02297f3c.id[i] = data_02297f3c.newId[i];
          data_02297f3c.flags[i] |= 4;
          data_02297fa8[i] = 0;
          break;
        }
      }
    }
  }
  for (i = 0; i < SAVE_SLOT_COUNT; i++) {
    if (!(data_02297f3c.flags[i] & SAVE_SLOT_KEEP) &&
        data_02297f3c.id[i] != 0xff) {
      data_02297f3c.newId[i] = 0xff;
      data_02297f3c.id[i] = data_02297f3c.newId[i];
    }
  }
}
