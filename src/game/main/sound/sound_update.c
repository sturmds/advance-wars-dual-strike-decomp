#include "types.h"

void func_02104e24(void);                   /* NitroSystem NNS_SndMain */

extern BOOL gSoundReady;

/* Runs the NitroSystem sound update once the sound system is ready. */
void Sound_Update(void)
{
    if (gSoundReady == 0) {
        return;
    }
    func_02104e24();
}
