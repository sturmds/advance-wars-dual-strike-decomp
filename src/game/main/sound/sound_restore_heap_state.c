#include "types.h"

int func_02106a6c(void *heap);              /* NitroSystem SoundHeap_NewestSaveLevel */
void func_02106a78(void *heap, int level);  /* NitroSystem NNS_SndHeapLoadState */

extern void *gSoundHeap;
extern BOOL gSoundReady;

/* Frees what was loaded into the sound heap since the last saved state.
 * Level 0 (nothing saved) is left alone. */
void Sound_RestoreHeapState(void)
{
    int level;

    if (gSoundReady == 0) {
        return;
    }
    level = func_02106a6c(gSoundHeap);
    if (level == 0) {
        return;
    }
    func_02106a78(gSoundHeap, level);
}
