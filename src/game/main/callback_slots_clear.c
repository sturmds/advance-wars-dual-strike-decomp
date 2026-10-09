#include "types.h"

extern void (*data_0216e2b8[16])(void);

/* Empties the slot table filled by CallbackSlots_Add. */
void CallbackSlots_Clear(void)
{
    u8 i;

    for (i = 0; i < 16; i++) {
        data_0216e2b8[i] = 0;
    }
}
