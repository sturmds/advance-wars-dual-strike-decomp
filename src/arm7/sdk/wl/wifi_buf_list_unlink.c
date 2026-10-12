#include "types.h"
#include "sdk/wl.h"

#define WIFI_LIST_LOCK_IRQS 0x01000000

u32 func_arm7_037fb654(u32 mask); /* ARM7 OS_DisableIrqMask */
u32 func_arm7_037fb690(u32 mask); /* ARM7 OS_EnableIrqMask */

/* Takes a buffer out of a list. Returns 1 if it is not a WL buffer, 2 if it is in another list, else 0. */
int WifiBufList_Unlink(WlBufferList *list, WlBuffer *buffer)
{
    u32 irqState;

    if (buffer->magic != WIFI_MSG_BUFFER_TAG) {
        return 1;
    }
    if (buffer->listId != list->id) {
        return 2;
    }
    irqState = func_arm7_037fb654(WIFI_LIST_LOCK_IRQS);
    list->count--;
    if (list->count == 0) {
        list->first = WIFI_LIST_END;
        list->last = WIFI_LIST_END;
    } else if (buffer == list->first) {
        list->first = buffer->next;
        list->first->prev = WIFI_LIST_END;
    } else if (buffer == list->last) {
        list->last = buffer->prev;
        list->last->next = WIFI_LIST_END;
    } else {
        buffer->next->prev = buffer->prev;
        buffer->prev->next = buffer->next;
    }
    buffer->listId = 0;
    func_arm7_037fb690(irqState);
    return 0;
}
