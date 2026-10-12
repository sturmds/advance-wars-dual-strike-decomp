#include "types.h"
#include "sdk/tp.h"

/* V alarm handler for auto sampling: queues one sample for the SPI thread.
 * When the queue is full, publishes a sample marked invalid instead, so the
 * ARM9 still gets a reply for this slot. */
void Touch_QueueAutoSample(void *arg)
{
    SPITpData sample;

    if (!SPIi_SetEntry(SERIAL_OWNER_TOUCH, TP_REQUEST_COMMAND_AUTO_SAMPLING, 1, (u32)arg)) {
        sample.e.validity = TP_VALIDITY_INVALID_XY;
        TOUCH_SHARED_LATEST_LOW = sample.halfs[0];
        TOUCH_SHARED_LATEST_HIGH = sample.halfs[1];
        func_arm7_03803e84(TP_REQUEST_COMMAND_AUTO_SAMPLING, (u32)arg & 0xff);
    }
}
