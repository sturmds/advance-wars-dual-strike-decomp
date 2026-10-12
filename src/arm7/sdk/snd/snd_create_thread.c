#include "sdk/snd_drv.h"

/* OS_CreateThread and OS_WakeupThreadDirect, by their arguments and the ARM9 copies. */
void Thread_Spawn_Arm7(OSThread *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 priority);
void Thread_MakeReady_Arm7(OSThread *thread);

void SndDrv_Thread(void *arg);

#define SOUND_THREAD_STACK_BYTES 0x400

extern OSThread data_arm7_03807bf0; /* the driver thread */
/* The driver thread's stack grows down from here; the stack has no symbol of its own. */
extern u8 data_arm7_03808068[];

void SND_CreateThread(u32 priority)
{
    Thread_Spawn_Arm7(&data_arm7_03807bf0, SndDrv_Thread, NULL, data_arm7_03808068, SOUND_THREAD_STACK_BYTES, priority);
    Thread_MakeReady_Arm7(&data_arm7_03807bf0);
}
