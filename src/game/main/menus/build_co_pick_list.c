#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"

#define CO_PICK_ORDER_LENGTH 27
#define CO_PICK_LIST_END     0xff

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* CO ids in the order the pick screen lists them. */
extern u8 gCoPickOrder[CO_PICK_ORDER_LENGTH];
/* The pick list, ended by CO_PICK_LIST_END; see GetCoPickList. */
extern u8 data_022a7298[];

u8 Unlock_IsSet(u32 id);

/* Fills the pick list with the unlocked COs of gCoPickOrder, in that order.
 * In gameMode 2 it adds CO_NONE as a last choice. Returns the count, not
 * counting the end marker. */
u8 BuildCoPickList(void)
{
    u8 count = 0;
    u8 i;

    for (i = 0; i < CO_PICK_ORDER_LENGTH; i++) {
        if (Unlock_IsSet(gCoPickOrder[i])) {
            data_022a7298[count++] = gCoPickOrder[i];
        }
    }
    if (data_027c027c->unk_48[data_027c0284]->gameMode == 2) {
        data_022a7298[count++] = CO_NONE;
    }
    data_022a7298[count] = CO_PICK_LIST_END;
    return count;
}
