#include "sdk/wl.h"

void func_arm7_027c8714(void); /* cancels the periodic alarm */
void func_arm7_027c8594(void); /* cancels the one-shot alarm */
void func_arm7_037f9380(int mode);
int func_arm7_027d00b4(void);
void func_arm7_027d02d8(void);
void WifiBuffer_ReleaseWorkLists(void);

/* Stops the current session: alarms off, mode 32, wireless interrupts and
   transmit requests cleared, pending frames freed. */
void Wifi_CloseSession(void)
{
    WlLink *link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);

    func_arm7_027c8714();
    func_arm7_027c8594();
    func_arm7_037f9380(WIFI_MODE_READY);
    link->unk_a4 = 0;
    link->unk_12 = 0;
    WIFI_IRQ_ENABLE_MASK = 0;
    WIFI_MAC_RUN = 0;
    WIFI_MAC_REGISTER(0x0ea) = 0;
    WIFI_MAC_REGISTER(0x0e8) = 0;
    WIFI_MAC_REGISTER(0x008) = 0;
    WIFI_MAC_REGISTER(0x00a) = 0;
    if (link->unk_0c == 1) {
        func_arm7_027d00b4();
    }
    REG_W_TXREQ_RESET = 0xffff;
    WIFI_MAC_REGISTER(0x0b4) = 0xffff;
    func_arm7_027d02d8();
    WifiBuffer_ReleaseWorkLists();
}
