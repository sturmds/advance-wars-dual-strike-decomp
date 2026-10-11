/* Needed to match: with loop-invariant motion on, mwcc swaps the registers
 * of the freeCount - 1 bound and the freeSlots[j] pointer in the slot sort. */
#pragma opt_loop_invariants off

#include "types.h"
#include "game/save_data.h"

extern u8 *data_02297f28;
extern int (*data_02297f2c)(u8 *dest);
extern int data_02297f30;
extern u32 gNextSaveCounter;
extern SaveSlotTable data_02297f3c;
extern SaveBuffer data_02298050;
extern u8 data_0229a050[];

void Save_WriteToBackup(u16 slot, SaveBuffer *buffer, int size);
int Save_VerifyBackup(u16 slot, SaveBuffer *buffer, int size);

/* Writes record recordId (size bytes from data_02297f28) to the save. The
 * record is split into parts of up to 0x2f99 bytes; the first part of
 * record 0 leaves room for the slot id table. Each part goes to the free
 * slot (id 0xff) that was written least often, after its write count goes
 * up. A slot that fails the read-back twice is marked
 * SAVE_SLOT_WRITE_FAILED and the next free slot is tried. Slots that held
 * the old copy of the record, and those of record 0, are freed (newId
 * 0xff). Returns 1 when no free slot is left. After any record but 0, 10
 * and 11, record 0 is written again from data_02297f2c.
 * The src argument is not read. On success the save counter goes into the
 * slot's write count, not into data_02297fa8[slot]; the code does that. */
int Save_WriteRecord(int recordId, u8 *unused, int size)
{
    u8 freeCount;
    int slot;
    int i;
    u8 freeSlots[SAVE_SLOT_COUNT];
    u8 tries;
    int chunk;
    u8 sum;
    u8 written[SAVE_SLOT_COUNT];
    int partLength[SAVE_SLOT_COUNT];
    int partOffset[SAVE_SLOT_COUNT];
    u8 *dst;
    SaveSlotTable *entry;
    int offset;
    int partHigh;
    int j;
    int part;
    u8 tmp;

    slot = 0;
    freeCount = 0;
    for (i = 0; i < SAVE_SLOT_COUNT; i++) {
        if (data_02297f3c.id[i] == recordId || data_02297f3c.id[i] == 0) {
            data_02297f3c.newId[i] = 0xff;
        } else if (data_02297f3c.id[i] == 0xff) {
            freeSlots[freeCount] = i;
            freeCount++;
        }
        written[i] = 0;
    }
    for (i = 0; i < freeCount - 1; i++) {
        for (j = i + 1; j < freeCount; j++) {
            if (SAVE_SLOT_WRITES(freeSlots[i]) < SAVE_SLOT_WRITES(freeSlots[j])) {
                tmp = freeSlots[j];
                freeSlots[j] = freeSlots[i];
                freeSlots[i] = tmp;
            }
        }
    }

    while (1) {
        offset = 0;
        data_02297f30 = 0;
        while (size != 0) {
            if (data_02297f30 == 0 && recordId == 0) {
                chunk = 0x2f84;
            } else {
                chunk = 0x2f99;
            }
            if (chunk > size) {
                chunk = size;
            }
            {
                /* Needed to match: the count is read once, then stored back. */
                int partCount = data_02297f30;

                partLength[partCount] = chunk;
                partOffset[partCount] = offset;
                data_02297f30 = partCount + 1;
            }
            size -= chunk;
            offset += chunk;
        }

        part = data_02297f30 - 1;
        tries = 0;
        if (part >= 0) {
            partHigh = part << 4;
            do {
                if (tries == 0 || tries == 4) {
                    if (tries != 0) {
                        data_02297f3c.flags[slot] |= SAVE_SLOT_WRITE_FAILED;
                        tries = 0;
                    }
                    if (freeCount == 0) {
                        for (i = 0; i < SAVE_SLOT_COUNT; i++) {
                            data_02297f3c.newId[i] = data_02297f3c.id[i];
                        }
                        return 1;
                    }
                    freeCount--;
                    slot = freeSlots[freeCount];
                }
                if (SAVE_SLOT_WRITES(slot) != 0xffffffff) {
                    SAVE_SLOT_WRITES(slot)++;
                }
                /* Needed to match: the table seen from slot, so that
                 * entry->flags[0] is data_02297f3c.flags[slot]. */
                entry = (SaveSlotTable *)&data_02297f3c.id[slot];
                entry->newId[0] = recordId;
                dst = (u8 *)&data_02298050;
                for (i = 0; i < 0x3000; i++) {
                    *dst++ = 0xff;
                }
                data_02298050.magic = SAVE_MAGIC;
                if (entry->unk_54[0] == 0x55) {
                    data_02298050.unk_04 = 0x55;
                    data_0229a050[0xfff] = 0xaa;
                } else {
                    data_02298050.unk_04 = 0xaa;
                    data_0229a050[0xfff] = 0x55;
                }
                data_02298050.unk_05 = 29;
                data_02298050.checksumInv = 0xff;
                data_02298050.checksum = 0;
                data_02298050.recordId = recordId;
                data_02298050.counter = gNextSaveCounter;
                data_02298050.part = partHigh + data_02297f30 - 1;
                data_02298050.destOffset = partOffset[part];
                for (i = 0; i < SAVE_SLOT_COUNT; i++) {
                    data_02298050.slotWrites[i] = SAVE_SLOT_WRITES(i);
                }
                /* size is 0 here; the original reuses it for the part length. */
                size = partLength[part];
                data_02298050.dataLength = size;
                for (i = 0; i < size; i++) {
                    data_02298050.data[i] = (data_02297f28 + partOffset[part])[i];
                }
                if (part == 0 && recordId == 0) {
                    for (i = 0; i < SAVE_SLOT_COUNT; i++) {
                        data_02297f3c.id[i] = data_02297f3c.newId[i];
                    }
                    if (gNextSaveCounter == 0) {
                        for (i = 0; i < SAVE_SLOT_COUNT; i++) {
                            if (written[i] == 0 && i != slot) {
                                data_02298050.slotIds[i] = 0xff;
                            } else {
                                data_02298050.slotIds[i] = data_02297f3c.id[i];
                            }
                        }
                    } else {
                        for (i = 0; i < SAVE_SLOT_COUNT; i++) {
                            data_02298050.slotIds[i] = data_02297f3c.id[i];
                        }
                    }
                }
                sum = 0;
                dst = (u8 *)&data_02298050;
                for (i = 0; i < 0x3000; i++) {
                    sum += *dst++;
                }
                data_02298050.checksumInv = ~sum;
                data_02298050.checksum = sum;
                Save_WriteToBackup(slot, &data_02298050, 0x3000);
                if (Save_VerifyBackup(slot, &data_02298050, 0x3000)) {
                    tries = 0;
                    entry->flags[0] |= SAVE_SLOT_KEEP;
                    part--;
                    entry->flags[0] &= ~SAVE_SLOT_READ_FAILED;
                    entry->part[0] = data_02298050.part;
                    entry->unk_54[0] = data_0229a050[0xfff];
                    written[slot] = 1;
                    SAVE_SLOT_WRITES(slot) = data_02298050.counter;
                    partHigh -= 0x10;
                } else {
                    entry->newId[0] = 0xff;
                    entry->unk_54[0] = data_0229a050[0xfff];
                    tries += 2;
                }
            } while (part >= 0);
        }

        if (gNextSaveCounter < 0xffffffff) {
            gNextSaveCounter++;
        }
        if (recordId == 0 || recordId == 10 || recordId == 11) {
            break;
        }
        recordId = 0;
        size = data_02297f2c(data_02297f28);
    }
    return 0;
}
