#include "types.h"
#include "sdk/wm.h"

void DC_InvalidateRange(void *start, u32 size);

/* Bytes for the MP send buffer, rounded up to 32. 0 when the state is wrong or
 * status field 0x0c is 1. */
u32 WM_GetMPSendBufferSize(void)
{
    WMArm9Buf *buf = func_02116e10();

    if (WMi_CheckStateEx(2, WM_STATE_PARENT, WM_STATE_CHILD) != 0) {
        return 0;
    }
    DC_InvalidateRange(&buf->status->unk_0c, 4);
    if (buf->status->unk_0c == 1) {
        return 0;
    }
    DC_InvalidateRange(&buf->status->unk_18e, 4);
    return (buf->status->unk_18e + 31) & ~31;
}
