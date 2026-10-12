#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wmsp.h"

extern OSThread data_arm7_0380749c;
extern OSThread data_arm7_03807424;

int func_arm7_027c7aa0(void);
int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

void Wmsp_SetNormalThreadPriorities(void)
{
    int irqState = func_arm7_037fdb80();

    Thread_HoldSwitching_Arm7();
    Thread_Reprioritize_Arm7(&data_arm7_03807424, data_arm7_027d7c44.threadPriority[1][2]);
    Thread_Reprioritize_Arm7((OSThread *)func_arm7_027c7aa0(), data_arm7_027d7c44.threadPriority[1][1]);
    Thread_Reprioritize_Arm7(&data_arm7_0380749c, data_arm7_027d7c44.threadPriority[1][0]);
    Thread_ReleaseSwitching_Arm7();
    func_arm7_037fdb94(irqState);
}
