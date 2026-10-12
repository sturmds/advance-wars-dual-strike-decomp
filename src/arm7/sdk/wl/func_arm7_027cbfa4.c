#include "types.h"

void WifiHost_TellBeaconLost(int, int);

void func_arm7_027cbfa4(void)
{
    int t0 = *((int *)0x380fff4);
    WifiHost_TellBeaconLost(t0 + 0x39a, t0);
}
