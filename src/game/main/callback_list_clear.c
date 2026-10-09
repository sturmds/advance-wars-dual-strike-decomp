#include "types.h"

extern s16 data_0216e2a0;
extern void (*gCallbackList[16])(void);

/* Empties the list that CallbackList_Add fills. */
void CallbackList_Clear(void)
{
    u8 i;

    data_0216e2a0 = 0;
    for (i = 0; i < 16; i++) {
        gCallbackList[i] = 0;
    }
}
