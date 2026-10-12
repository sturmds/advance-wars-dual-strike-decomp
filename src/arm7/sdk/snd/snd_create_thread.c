#include "sdk/os_arm7.h"
#include "sdk/snd.h"

/* OS_CreateThread and OS_WakeupThreadDirect, by their arguments and the ARM9 copies. */

#define SOUND_THREAD_STACK_BYTES 0x400

extern OSThread data_arm7_03807bf0; /* the driver thread */

void SND_CreateThread(u32 priority) {
  /* The stack has no symbol of its own: it grows down from the locked channel mask that follows it. */
  Thread_Spawn_Arm7(&data_arm7_03807bf0, SndDrv_Thread, NULL,
                    &data_arm7_03808068, SOUND_THREAD_STACK_BYTES, priority);
  Thread_MakeReady_Arm7(&data_arm7_03807bf0);
}
