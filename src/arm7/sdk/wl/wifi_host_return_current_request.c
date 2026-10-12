#include "types.h"
#include "sdk/wl.h"

/* Hands the current host request back to the host and queues task 11 again if more requests wait in the inbox. */
void WifiHost_ReturnCurrentRequest(void)
{
    WlWork *work = WIFI_DRIVER_STATE;
    /* needed to match: the original reaches inbox through a pointer to the buffer fields */
    WlBufferPool *pool = (WlBufferPool *)&work->allocator;
    WlCurrentRequest *current = &work->state.current;

    current->busy &= ~1;
    WifiBuffer_GiveToHost(&pool->inbox, current->buffer);
    if (pool->inbox.count != 0) {
        WifiTask_Schedule(2, 11);
    }
}
