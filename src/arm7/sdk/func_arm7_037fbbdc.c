#include "types.h"
#include "sdk/os_arm7.h"

void Thread_MakeReady_Arm7(OSThread *thread);

/* Alarm handler of Thread_NapMilliseconds_Arm7: wakes the sleeping thread. */
void func_arm7_037fbbdc(void *thread)
{
    Thread_MakeReady_Arm7(thread);
}
