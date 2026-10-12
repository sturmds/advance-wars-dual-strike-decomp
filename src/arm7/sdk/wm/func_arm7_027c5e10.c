#include "sdk/wmsp.h"

void func_arm7_027c5e10(WmspRequest *request)
{
    if (func_arm7_027c05e0()->buf->status->state == WM_STATE_MP_PARENT) {
        func_arm7_027c6eb4(request->arg);
    }
}
