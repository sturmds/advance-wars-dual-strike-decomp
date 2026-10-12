#include "types.h"
#include "sdk/spi_thread.h"

BOOL OS_ReadMessage(OSMessageQueue *queue, OSMessage *msg, int flags);

/* TRUE when a job waits in the SPI queue. Peeks only: the job stays queued. */
BOOL SPIi_CheckEntry(void)
{
    OSMessage msg;

    return OS_ReadMessage(&data_arm7_03809ab0.msgQ, &msg, OS_MESSAGE_NOBLOCK);
}
