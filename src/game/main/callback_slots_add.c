#include "types.h"

extern void (*data_0216e2b8[16])(void);
extern void (*data_0216e2f8[16])(void);

/* Stores the callback in the first free slot of the 16-slot table
 * data_0216e2b8 (which sits right before data_0216e2f8); ignored when full. */
void CallbackSlots_Add(void (*callback)(void))
{
    void (**slot)(void);

    for (slot = data_0216e2b8; slot < data_0216e2f8; slot++) {
        if (*slot == 0) {
            *slot = callback;
            return;
        }
    }
}
