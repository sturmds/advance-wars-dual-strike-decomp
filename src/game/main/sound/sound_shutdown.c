#include "types.h"
#include "game/sound.h"

extern void *data_0218feb0;                 /* sound heap set up by func_0200b83c / func_0200b8d0 */

/* Stops every sequence at once, destroys the sound heap and marks the
 * sound system as not ready, so the other Sound_ calls do nothing. */
void Sound_Shutdown(void)
{
    if (gSoundReady == 0) {
        return;
    }
    NNS_SndPlayerStopSeqAll(0);
    NNS_SndHeapDestroy(data_0218feb0);
    gSoundReady = FALSE;
}
