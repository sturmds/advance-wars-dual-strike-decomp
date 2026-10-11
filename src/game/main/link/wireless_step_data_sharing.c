#include "types.h"

extern void *data_0229c8ac;
extern u16 *data_0229c8b0;

int WM_StepDataSharing(void *sharing, void *sendData, u16 *receiveBuffer);

/* Sends one 8-byte block through the data sharing session and, when the step
 * succeeds, copies the u16 at +2 of the received buffer to *out. True on success. */
BOOL Wireless_StepDataSharing(void *sendData, u16 *out)
{
    BOOL ok;

    if (WM_StepDataSharing(data_0229c8ac, sendData, data_0229c8b0) != 0) {
        ok = FALSE;
    } else {
        *out = data_0229c8b0[1];
        ok = TRUE;
    }
    return ok;
}
