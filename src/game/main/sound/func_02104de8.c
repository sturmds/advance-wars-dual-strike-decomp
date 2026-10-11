#include "types.h"

void SoundOutput_Pause(void);
void SND_StopTimer(int arg0, int arg1, u32 channelMask, int arg3);
int func_02110150(void);
void func_02110220(int arg0);
void func_021102b0(int arg0);

/* Callback that SoundSystem_Init adds to a system callback list. Pauses the
 * sound output, then calls the hardware routines not named yet. */
void func_02104de8(void)
{
    int saved;

    SoundOutput_Pause();
    SND_StopTimer(0, 0, 0, 0);
    saved = func_02110150();
    func_021102b0(1);
    func_02110220(saved);
}
