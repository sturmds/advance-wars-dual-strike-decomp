#include "stdarg.h"

#include "sdk/wm.h"

void DC_InvalidateRange(void *start, u32 size);

extern WMArm9Buf *data_022ac32c;

int WMi_CheckStateEx(int count, ...)
{
    int result;
    u16 state;
    va_list states;

    result = func_02116df4();
    if (result != WM_ERRCODE_SUCCESS) {
        return result;
    }

    DC_InvalidateRange(&data_022ac32c->status->state, 2);
    state = data_022ac32c->status->state;
    va_start(states, count);
    result = WM_ERRCODE_ILLEGAL_STATE;
    while (count != 0) {
        if (va_arg(states, u32) == state) {
            result = WM_ERRCODE_SUCCESS;
        }
        count--;
    }
    va_end(states);
    return result;
}
