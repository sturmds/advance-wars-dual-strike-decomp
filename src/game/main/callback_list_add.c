#include "types.h"

extern volatile s16 gCallbackCount;
extern void (*gCallbackList[16])(void);              /* list run and then cleared by CallbackList_RunAndClear */

/* Adds a callback to the list of up to 16; ignored when the list is full.
 * The count is volatile to match. */
void CallbackList_Add(void (*callback)(void))
{
    if (gCallbackCount == 16) {
        return;
    }
    gCallbackList[gCallbackCount++] = callback;
}
