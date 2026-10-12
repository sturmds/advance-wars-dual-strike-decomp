#include "types.h"

void Alarm_Withdraw_Arm7(int, int);

void func_arm7_027c8714(void)
{
    int t0 = *((int *)0x380fff4);
    Alarm_Withdraw_Arm7(t0 + 0x5dc, t0);
}
