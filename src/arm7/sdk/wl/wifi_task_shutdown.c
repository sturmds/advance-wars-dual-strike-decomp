#include "sdk/wl.h"

void WifiIrq_Uninstall(void);

/* Task 23: frees the wifi config and the unk_318 data block, then ends the driver thread. */
void WifiTask_Shutdown(void)
{
    WlBufferPool *pool = (WlBufferPool *)&WIFI_DRIVER_STATE->allocator;

    WifiIrq_Uninstall();
    /* - 1: back from the data to the buffer header */
    WifiBuffer_Release(&pool->newBuffers, (WlBuffer *)WIFI_DRIVER_STATE->limits.wifiConfig - 1);
    WifiBuffer_Release(&pool->newBuffers, (WlBuffer *)WIFI_DRIVER_STATE->unk_318.data - 1);
    Thread_EndCurrent_Arm7();
}
