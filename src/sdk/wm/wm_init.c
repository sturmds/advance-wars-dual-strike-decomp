#include "sdk/wm.h"

extern WMArm9Buf *data_022ac32c;

/* workArea needs WM_SYSTEM_BUF_SIZE bytes, 32-byte aligned. */
int WM_Init(void *workArea, u16 dmaNo)
{
    int result = Wireless_SetUpWorkArea(workArea, dmaNo, WM_SYSTEM_BUF_SIZE);

    if (result == WM_ERRCODE_SUCCESS) {
        data_022ac32c->scanOnlyFlag = FALSE;
    }
    return result;
}
