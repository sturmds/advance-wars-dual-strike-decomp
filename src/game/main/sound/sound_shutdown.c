#include "types.h"

void func_02105858(int fadeFrames);         /* NitroSystem NNS_SndPlayerStopSeqAll */
void func_02106cc4(void *heap);             /* NitroSystem NNS_SndHeapDestroy */

extern void *data_0218feb0;                 /* sound heap set up by func_0200b83c / func_0200b8d0 */
extern BOOL gSoundReady;

/* Stops every sequence at once, destroys the sound heap and marks the
 * sound system as not ready, so the other Sound_ calls do nothing. */
void Sound_Shutdown(void)
{
    if (gSoundReady == 0) {
        return;
    }
    func_02105858(0);
    func_02106cc4(data_0218feb0);
    gSoundReady = FALSE;
}
