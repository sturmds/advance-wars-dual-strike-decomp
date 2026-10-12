#include "sdk/wl.h"

void func_arm7_027c8594(void); /* cancels the one-shot alarm */
void func_arm7_027c9438(void); /* RF power down: sets WIFI_MAC_REGISTER(0x036) to 1 */
int func_arm7_027c94ac(int value); /* writes WIFI_MAC_FORCE_POWER */
void Wifi_CloseSession(void);
void func_arm7_027c93a4(void); /* RF power up and WifiRf_LoadStartValues */
void func_arm7_027c92b8(void);
void Wifi_ArmPeriodicMs(u32 milliseconds, OSAlarmHandler handler);
void Wifi_EveryHundredMs(void *arg);

/* Moves WlLink.mode to mode, doing the work each target mode needs. Mode 64
   runs the periodic alarm; leaving it cancels the one-shot alarm. */
void Wifi_SwitchMode(int mode)
{
    WlLink *link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);

    if (link->mode == mode) {
        return;
    }
    if (link->mode == 64) {
        func_arm7_027c8594();
    }
    switch (mode) {
    case 0:
        func_arm7_027c9438();
        break;
    case WIFI_MODE_RADIO_ON:
        func_arm7_027c94ac(0);
        Wifi_CloseSession();
        func_arm7_027c93a4();
        break;
    case 64:
        if (link->unk_0c == 2) {
            func_arm7_027c92b8();
        }
        Wifi_ArmPeriodicMs(100, Wifi_EveryHundredMs);
        break;
    }
    link->mode = mode;
}
