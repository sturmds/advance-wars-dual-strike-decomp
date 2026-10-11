#include "types.h"

extern volatile s16 data_0216e2a0;                   /* number of callbacks in the list */
extern void (*gCallbackList[16])(void);              /* list run and then cleared by CallbackList_RunAndClear */

/* Adds a callback to the list of up to 16; ignored when the list is full.
 * The count is volatile to match. */
void CallbackList_Add(void (*callback)(void))
{
    if (data_0216e2a0 == 16) {
        return;
    }
    gCallbackList[data_0216e2a0++] = callback;
}
