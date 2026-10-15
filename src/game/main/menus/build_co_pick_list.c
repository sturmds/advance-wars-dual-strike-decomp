#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"
#include "game/unlocks.h"

#define CO_PICK_ORDER_LENGTH 27
#define CO_PICK_LIST_END 0xff

/* CO ids in the order the pick screen lists them. */
extern u8 gCoPickOrder[CO_PICK_ORDER_LENGTH];
/* The pick list, ended by CO_PICK_LIST_END; see GetCoPickList. */
extern u8 gCoPickList[];

/* Fills the pick list with the unlocked COs of gCoPickOrder, in that order.
 * In gameMode 2 it adds CO_NONE as a last choice. Returns the count, not
 * counting the end marker. */
u8 BuildCoPickList(void) {
  u8 count = 0;
  u8 i;

  for (i = 0; i < CO_PICK_ORDER_LENGTH; i++) {
    if (Unlock_IsSet(gCoPickOrder[i])) {
      gCoPickList[count++] = gCoPickOrder[i];
    }
  }
  if (data_027c027c->unk_48[data_027c0284]->gameMode == 2) {
    gCoPickList[count++] = CO_NONE;
  }
  gCoPickList[count] = CO_PICK_LIST_END;
  return count;
}
