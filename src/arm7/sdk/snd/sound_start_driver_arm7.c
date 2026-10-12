#include "types.h"

void SndArm7_Init(void);
void SND_CreateThread(u32 priority);

extern BOOL gSndArm7Initialized;

/* Starts the ARM7 sound driver once: command queue, then the driver thread. */
void Sound_StartDriver_Arm7(u32 priority)
{
    if (!gSndArm7Initialized) {
        gSndArm7Initialized = TRUE;
        SndArm7_Init();
        SND_CreateThread(priority);
    }
}
