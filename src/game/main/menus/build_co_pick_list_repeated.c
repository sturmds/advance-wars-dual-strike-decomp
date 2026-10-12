#include "types.h"

#define CO_PICK_LIST_MIN_LENGTH 7
#define CO_PICK_LIST_END 0xff

u8 BuildCoPickList(void);
extern u8 gCoPickList[];

/* Builds the pick list, then copies it after itself until it holds at least
 * CO_PICK_LIST_MIN_LENGTH entries, likely so a short list can scroll round. */
void BuildCoPickListRepeated(void) {
  u8 count = BuildCoPickList();
  u8 i;

  while (count != 0 && count < CO_PICK_LIST_MIN_LENGTH) {
    for (i = 0; i < count; i++) {
      gCoPickList[count + i] = gCoPickList[i];
    }
    count += i;
  }
  gCoPickList[count] = CO_PICK_LIST_END;
}
