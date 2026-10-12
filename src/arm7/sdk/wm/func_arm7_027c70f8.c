#include "sdk/wmsp.h"

/* Needed to match: the early returns get their own epilogues, as -O4,p builds them. */
#pragma optimize_for_size off

int func_arm7_027c698c(int a0, int a1, int a2, int a3, u16 *flags, const void *buf, u32 *a6, u32 *size);
u16 *func_arm7_027c2344(WlCommand *cmd, u16 arg0, u16 arg1, u32 arg2);

void func_arm7_027c70f8(void)
{
    WMStatus *status = func_arm7_027c05e0()->buf->status;
    const void *buf;
    u16 port;
    u16 flags;
    u32 unused;
    u32 size;
    u32 cmd[0x80];

    if (status->childBitmap == 0) {
        return;
    }
    buf = status->unk_50;
    port = status->unk_18e;
    flags = 0;
    if (func_arm7_027c698c(1, 1, 1, port, &flags, buf, &unused, &size) == 1) {
        return;
    }
    if (status->unk_1c == 1) {
        flags |= 0x8000;
    }
    if (func_arm7_027c2344((WlCommand *)cmd, size, flags, (u32)buf)[2] == 0) {
        status->unk_8a = 1;
    }
}
