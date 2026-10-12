#include "types.h"

void Alarm_Withdraw_Arm7(int, int);

void func_arm7_027c8594(void)
{
    int t0 = *((int *)0x380fff4);
    Alarm_Withdraw_Arm7(t0 + 0x608, t0);
}
