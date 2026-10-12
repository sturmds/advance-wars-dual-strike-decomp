#include "sdk/wl.h"

void func_arm7_027cab1c(void)
{
    WlWork *work;
    u16 saved;

    work = WIFI_DRIVER_STATE;
    if (work->savedMpReply != 0xffff) {
        saved = REG_W_TXBUF_REPLY1;
        REG_W_TXBUF_REPLY1 = work->savedMpReply;
        REG_W_RXCNT |= W_RXCNT_COPY_REPLY;
        REG_W_TXBUF_REPLY1 = saved;
        work->savedMpReply = 0xffff;
    }
}
