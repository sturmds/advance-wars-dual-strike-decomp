#include "sdk/wl.h"

void Wifi_ClearSessionArea(u32 value, int limit);
void func_arm7_027c8594(void);
void func_arm7_027c9438(void);
void WifiMac_LoadRegisterTable(void);
void func_arm7_027c7dd4(void);
void func_arm7_027c7cf4(void);
void func_arm7_027cb920(void);
void func_arm7_027cb9ac(void);
void func_arm7_027cacec(void);
void func_arm7_027c98d8(void);

/* Part of the setup Wifi_StartDriver does, again, with the values Wifi_StartDriver stored. */
void Wifi_RestartDriver(void)
{
    func_arm7_027c8594();
    func_arm7_027c9438();
    WifiMac_LoadRegisterTable();
    func_arm7_027c7dd4();
    func_arm7_027c7cf4();
    Wifi_ClearSessionArea(WIFI_DRIVER_STATE->limits.unk_f0, WIFI_DRIVER_STATE->limits.unk_f4);
    func_arm7_027cb920();
    func_arm7_027cb9ac();
    func_arm7_027cacec();
    func_arm7_027c98d8();
}
