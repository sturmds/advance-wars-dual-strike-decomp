#include "types.h"
#include "game/save_data.h"

extern SaveSlotTable data_02297f3c;

/* Finds the newest complete copy of record recordId in the save slots. A
 * record is written over several slots, one per part. The copy with the
 * highest save counter is taken if each of its parts 0 to last is held by
 * exactly one slot with that counter; otherwise slots with that counter are
 * skipped and the search repeats. Marks the chosen copy's slots with
 * SAVE_SLOT_MARK and returns the slot of the first one found, or 0xffff if
 * none is complete.
 * recordId 0xff looks for record 0 slots whose counter is 0. */
u16 Save_FindNewestRecord(int recordId) {
  u8 partCount[SAVE_SLOT_COUNT];
  u8 ids[SAVE_SLOT_COUNT];
  int target;
  int best;
  int i;
  int j;
  int part;
  u32 counter;

  target = recordId == 0xff ? 0 : recordId;
  for (i = 0; i < SAVE_SLOT_COUNT; i++) {
    ids[i] = data_02297f3c.id[i];
    data_02297f3c.flags[i] &= ~SAVE_SLOT_MARK;
  }

  while (1) {
    best = -1;
    for (i = 0; i < SAVE_SLOT_COUNT; i++) {
      if (ids[i] != target) {
        continue;
      }
      /* Needed to match: the repeated recordId test. */
      if (recordId == 0xff && !(recordId == 0xff && data_02297fa8[i] == 0)) {
        continue;
      }
      if (best == -1) {
        best = i;
        for (j = 0; j < SAVE_SLOT_COUNT; j++) {
          partCount[j] = 0;
          data_02297f3c.flags[j] &= ~SAVE_SLOT_MARK;
        }
        partCount[data_02297f3c.part[i] >> 4]++;
        data_02297f3c.flags[i] |= SAVE_SLOT_MARK;
      } else if (best > -1 && /* needed to match: always true here */
                 data_02297fa8[best] <= data_02297fa8[i]) {
        if (data_02297fa8[best] == data_02297fa8[i]) {
          partCount[data_02297f3c.part[i] >> 4]++;
          data_02297f3c.flags[i] |= SAVE_SLOT_MARK;
        } else {
          best = i;
          for (j = 0; j < SAVE_SLOT_COUNT; j++) {
            partCount[j] = 0;
            data_02297f3c.flags[j] &= ~SAVE_SLOT_MARK;
          }
          partCount[data_02297f3c.part[i] >> 4]++;
          data_02297f3c.flags[i] |= SAVE_SLOT_MARK;
        }
      }
    }
    if (best == -1) {
      return 0xffff;
    }

    /* Needed to match: i holds the last part index here, and the cast
         * keeps the test before the first pass of the loop. */
    i = (int)(data_02297f3c.part[best] & 0xf);
    for (part = 0; part < i + 1; part++) {
      if (partCount[part] != 1) {
        break;
      }
    }
    if (part == i + 1) {
      for (; part < SAVE_SLOT_COUNT; part++) {
        if (partCount[part] != 0) {
          break;
        }
      }
      if (part == SAVE_SLOT_COUNT) {
        return best;
      }
    }

    counter = data_02297fa8[best];
    for (i = 0; i < SAVE_SLOT_COUNT; i++) {
      if (ids[i] == target && counter == data_02297fa8[i]) {
        ids[i] = 0xff;
      }
      data_02297f3c.flags[i] &= ~SAVE_SLOT_MARK;
    }
  }
}
