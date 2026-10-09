#include "types.h"

extern void (*gCallbackSlots[16])(void);   /* callbacks run by func_020044c4 */

/* Removes a callback from the list of 16; nothing happens if it is not there. */
void FrameCallback_Remove(void (*callback)(void))
{
    u8 i;

    for (i = 0; i < 16; i++) {
        if (gCallbackSlots[i] == callback) {
            gCallbackSlots[i] = NULL;
            return;
        }
    }
}
