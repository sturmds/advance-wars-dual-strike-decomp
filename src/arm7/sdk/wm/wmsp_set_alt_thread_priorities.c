#include "types.h"
#include "sdk/os.h"
#include "sdk/wmsp.h"

extern OSThread data_arm7_0380749c;
extern OSThread data_arm7_03807424;

int func_arm7_027c7aa0(void);
int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void Thread_ReleaseSwitching_Arm7(void);
int Thread_HoldSwitching_Arm7(void);
BOOL func_arm7_037fbc48(OSThread *thread, u32 priority);

void Wmsp_SetAltThreadPriorities(void)
{
    int irqState = func_arm7_037fdb80();

    Thread_HoldSwitching_Arm7();
    func_arm7_037fbc48(&data_arm7_0380749c, data_arm7_027d7c44.threadPriority[0][0]);
    func_arm7_037fbc48((OSThread *)func_arm7_027c7aa0(), data_arm7_027d7c44.threadPriority[0][1]);
    func_arm7_037fbc48(&data_arm7_03807424, data_arm7_027d7c44.threadPriority[0][2]);
    Thread_ReleaseSwitching_Arm7();
    func_arm7_037fdb94(irqState);
}
