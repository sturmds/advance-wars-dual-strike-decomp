#include "types.h"

int func_02106b50(void *heap);              /* NitroSystem NNS_SndHeapSaveState */

extern void *gSoundHeap;
extern BOOL gSoundReady;

/* Saves the sound heap state, so Sound_RestoreHeapState can later free
 * everything loaded after this point. */
void Sound_SaveHeapState(void)
{
    if (gSoundReady == 0) {
        return;
    }
    func_02106b50(gSoundHeap);
}
