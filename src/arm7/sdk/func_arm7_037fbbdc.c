#include "types.h"
#include "sdk/os_arm7.h"

void func_arm7_037fbdf8(OSThread *thread);

/* Alarm handler of Thread_NapMilliseconds_Arm7: wakes the sleeping thread. */
void func_arm7_037fbbdc(void *thread)
{
    func_arm7_037fbdf8(thread);
}
