#include "sdk/wmsp.h"
#include "sdk/mi.h"

/* Needed to match: the zero goes through a halfword on the stack. */
static inline void MI_CpuClear16(void *dest, u32 size)
{
    volatile u16 zero = 0;

    Mem_FillHalfwords_Arm7(zero, dest, size);
}

/* Links all port send entries into the free list and empties the send and
   ready queues of each priority. */
void WMSP_InitSendQueue(void)
{
    WMStatus *status = func_arm7_027c05e0()->buf->status;
    u16 i;

    Mutex_Take_Arm7(&status->sendQueueMutex);
    MI_CpuClear16(status->sendQueueData, sizeof(status->sendQueueData));
    for (i = 0; i < WIRELESS_OUTBOX_ENTRIES - 1; i++) {
        status->sendQueueData[i].next = i + 1;
    }
    status->sendQueueData[i].next = WIRELESS_OUTBOX_NO_ENTRY;
    status->sendQueueFreeList.head = 0;
    status->sendQueueFreeList.tail = i;
    for (i = 0; i < WIRELESS_URGENCY_LEVELS; i++) {
        status->readyQueue[i].head = WIRELESS_OUTBOX_NO_ENTRY;
        status->readyQueue[i].tail = WIRELESS_OUTBOX_NO_ENTRY;
        status->sendQueue[i].head = WIRELESS_OUTBOX_NO_ENTRY;
        status->sendQueue[i].tail = WIRELESS_OUTBOX_NO_ENTRY;
    }
    Mutex_Release_Arm7(&status->sendQueueMutex);
}
