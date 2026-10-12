#include "types.h"
#include "sdk/wl.h"

#define WIFI_LIST_LOCK_IRQS 0x01000000

u32 func_arm7_037fb654(u32 mask); /* ARM7 OS_DisableIrqMask */
u32 func_arm7_037fb690(u32 mask); /* ARM7 OS_EnableIrqMask */

/* Adds a buffer at the end of a list. Returns 1 if it is not a WL buffer, 2 if it is in a list already, else 0. */
int WifiBufList_PushTailIfLoose(WlBufferList *list, WlBuffer *buffer)
{
    u32 irqState;

    if (buffer->magic != WIFI_MSG_BUFFER_TAG) {
        return 1;
    }
    if (buffer->listId != 0) {
        return 2;
    }
    irqState = func_arm7_037fb654(WIFI_LIST_LOCK_IRQS);
    if (list->count == 0) {
        buffer->prev = WIFI_LIST_END;
        list->first = buffer;
    } else {
        WlBuffer *last = list->last;

        buffer->prev = last;
        last->next = buffer;
    }
    buffer->next = WIFI_LIST_END;
    buffer->listId = list->id;
    list->last = buffer;
    list->count++;
    func_arm7_037fb690(irqState);
    return 0;
}
