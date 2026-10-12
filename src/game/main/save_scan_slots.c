#include "types.h"
#include "game/save_data.h"

extern u8* gSaveBuffer;
extern int gNextSaveCounter;
extern int data_02297f38;
extern SaveSlotTable data_02297f3c;
extern SaveBuffer data_02298050;
extern u8 data_0229a050[];

int Save_ReadSlot(u16 slot);
u16 Save_FindNewestRecord(int recordId);
void Save_Format(void);
int Save_LoadRecord(int recordId, void* buffer);
void Save_WriteRecord(int recordId, u8* src, int size);

/* Reads every save slot at startup (called once by Save_Init). Each slot
 * that reads back fine gives its record id, part byte and save counter; the
 * slot with the highest counter also gives the write counts of all slots and
 * the next save counter. With no readable slot, the save is formatted unless
 * noFormat is set; the same happens when record 0 has no complete copy.
 * The newest copy of record 0 then supplies the slot id table. If a slot
 * failed to read but that table still lists it as in use, record 0 is
 * loaded, the failed slots are freed (id 0xff) and record 0 is written
 * again (Save_WriteRecord). */
void Save_ScanSlots(int noFormat) {
  int bad[SAVE_SLOT_COUNT];
  int i; /* needed to match: i, newest, anyBad, j in this order, and the
            * separate j for the first loop */
  int newest;
  int anyBad;
  int j;
  int slot;
  int listedBad;

  newest = -1;
  for (j = 0; j < SAVE_SLOT_COUNT; j++) {
    SAVE_SLOT_WRITES(j) = 0;
  }
  gNextSaveCounter = 0;
  anyBad = 0; /* needed to match: set here, not before the loop above */
  for (i = 0; i < SAVE_SLOT_COUNT; i++) {
    bad[i] = 0;
    data_02297f3c.newId[i] = 0xff;
    data_02297f3c.id[i] = data_02297f3c.newId[i];
    data_02297f3c.flags[i] = 0;
    if (Save_ReadSlot(i) == 0) {
      data_02297fa8[i] = data_02298050.counter;
      data_02297f3c.newId[i] = data_02298050.recordId;
      data_02297f3c.id[i] = data_02297f3c.newId[i];
      data_02297f3c.part[i] = data_02298050.part;
      if (newest == -1 ||
          (newest > -1 && /* needed to match: always true here */
           (data_02297fa8[newest] < data_02298050.counter ||
            (data_02298050.counter == data_02297fa8[newest] &&
             data_02297f3c.part[newest] > data_02298050.part)))) {
        newest = i;
        for (j = 0; j < SAVE_SLOT_COUNT; j++) {
          SAVE_SLOT_WRITES(j) = data_02298050.slotWrites[j];
        }
        gNextSaveCounter = data_02298050.counter + 1;
      }
    } else {
      anyBad = 1;
      bad[i] = anyBad;
    }
    data_02297f3c.unk_54[i] = data_0229a050[0xfff];
  }

  if (newest == -1) {
    if (!noFormat) {
      Save_Format();
    }
    return;
  }

  slot = Save_FindNewestRecord(0);
  if (slot == 0xffff) {
    for (i = 0; i < SAVE_SLOT_COUNT; i++) {
      data_02297f3c.newId[i] = 0xff;
      data_02297f3c.id[i] = data_02297f3c.newId[i];
    }
    if (!noFormat) {
      Save_Format();
    }
    return;
  }
  for (i = 0; i < SAVE_SLOT_COUNT; i++) {
    if ((data_02297f3c.flags[i] & SAVE_SLOT_MARK) &&
        data_02297f3c.part[i] < data_02297f3c.part[slot]) {
      slot = i;
    }
  }
  Save_ReadSlot(slot);

  listedBad = 0;
  for (i = 0; i < SAVE_SLOT_COUNT; i++) {
    if (bad[i] && data_02298050.slotIds[i] != 0xff) {
      listedBad = 1;
    } else {
      bad[i] = 0;
    }
    data_02297f3c.newId[i] = data_02298050.slotIds[i];
    data_02297f3c.id[i] = data_02297f3c.newId[i];
  }
  if (!listedBad) {
    anyBad = 0;
  }
  if (!anyBad) {
    return;
  }

  Save_LoadRecord(0, NULL);
  for (i = 0; i < SAVE_SLOT_COUNT; i++) {
    if (bad[i]) {
      data_02298050.slotIds[i] = 0xff;
      data_02297f3c.newId[i] = data_02298050.slotIds[i];
      data_02297f3c.id[i] = data_02297f3c.newId[i];
    }
  }
  Save_WriteRecord(0, gSaveBuffer, data_02297f38);
}
