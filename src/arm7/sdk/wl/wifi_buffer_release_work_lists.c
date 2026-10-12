#include "sdk/wl.h"

/* Frees the buffers in lists 3 to 10; leaves newBuffers, the outbox and the inbox. */
void WifiBuffer_ReleaseWorkLists(void)
{
    /* needed to match: the original reaches the lists through a pointer to the buffer fields */
    WlBufferPool *pool = (WlBufferPool *)&WIFI_DRIVER_STATE->allocator;

    WifiBufList_ReleaseAll(&pool->lists[0]);
    WifiBufList_ReleaseAll(&pool->lists[1]);
    WifiBufList_ReleaseAll(&pool->lists[2]);
    WifiBufList_ReleaseAll(&pool->lists[3]);
    WifiBufList_ReleaseAll(&pool->lists[4]);
    WifiBufList_ReleaseAll(&pool->lists[5]);
    WifiBufList_ReleaseAll(&pool->lists[6]);
    WifiBufList_ReleaseAll(&pool->lists[7]);
}
