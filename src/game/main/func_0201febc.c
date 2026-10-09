#include "types.h"
#include "game/save_data.h"

int func_0201fadc(u16 slot);
int func_0201fe28(int recordId);

extern int data_02297f38;
extern u8 *data_02297f28;
extern SaveSlotTable data_02297f3c;
extern SaveBuffer data_02298050;

/* Loads the save record recordId (a guess: func_0201fe28 and func_0201fadc
 * look like a record check and a read from the save buffer). For each slot
 * that holds the record, the data in the save buffer is copied to
 * data_02297f28 + destOffset and its length is added to data_02297f38.
 * Record 0 also replaces the slot ids with the ones the buffer holds.
 * Returns 1 on failure.
 *
 * Not matching yet (97%): the register for the copy loop counter is r2 where
 * the original has r1. */
int func_0201febc(int recordId)
{
    int found;
    u8 savedMarks[SAVE_SLOT_COUNT];
    int i;

    data_02297f38 = 0;
    for (i = 0; i < SAVE_SLOT_COUNT; i++) {
        savedMarks[i] = data_02297f3c.flags[i] & SAVE_SLOT_MARK;
    }
    found = func_0201fe28(recordId);
    for (i = 0; i < SAVE_SLOT_COUNT; i++) {
        data_02297f3c.flags[i] = savedMarks[i] | (data_02297f3c.flags[i] & ~SAVE_SLOT_MARK);
    }
    if (found) {
        return 1;
    }

    for (i = 0; i < SAVE_SLOT_COUNT; i++) {
        int j;

        if (recordId == data_02297f3c.id[i]) {
            if (func_0201fadc(i)) {
                return 1;
            }
            data_02297f38 += data_02298050.dataLength;
            for (j = 0; j < data_02298050.dataLength; j++) {
                (data_02297f28 + data_02298050.destOffset)[j] = data_02298050.data[j];
            }
            if ((data_02298050.unk_0c & 0xf0) == 0 && recordId == 0) {
                for (j = 0; j < SAVE_SLOT_COUNT; j++) {
                    data_02297f3c.newId[j] = data_02298050.slotIds[j];
                }
            }
        }
    }
    if (recordId == 0) {
        for (i = 0; i < SAVE_SLOT_COUNT; i++) {
            data_02297f3c.id[i] = data_02297f3c.newId[i];
        }
    }
    return 0;
}
