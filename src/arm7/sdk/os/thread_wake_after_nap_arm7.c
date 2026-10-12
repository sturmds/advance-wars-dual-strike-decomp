#include "types.h"
#include "sdk/os_arm7.h"

/* Alarm handler of Thread_NapMilliseconds_Arm7: wakes the sleeping thread. */
void Thread_WakeAfterNap_Arm7(void *thread)
{
    Thread_MakeReady_Arm7(thread);
}
