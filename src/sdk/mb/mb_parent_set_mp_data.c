#include "types.h"
#include "sdk/mb.h"
#include "sdk/wm.h"

int func_02117e7c(void (*callback)(void), int arg, void *data, u32 size, int destBitmap, int port, int priority);
void MbParent_ReportError(u16 apiid, int errcode);

/* Queues data on port 1, priority 3, for the children in bitmap, and reports
 * a failure to the parent's error callback. unused is not read. */
int MbParent_SetMpData(void (*callback)(void), void *data, u16 size, u16 unused, u16 bitmap)
{
    int result = func_02117e7c(callback, 0, data, size, bitmap, 1, 3);

    MbParent_ReportError(WM_APIID_SET_MP_DATA, result);
    return result;
}
