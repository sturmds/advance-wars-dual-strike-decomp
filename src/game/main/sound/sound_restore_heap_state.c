#include "types.h"

int func_02106a6c(void *heap);              /* NitroSystem SoundHeap_NewestSaveLevel */
void func_02106a78(void *heap, int level);  /* NitroSystem NNS_SndHeapLoadState */

extern void *data_0218feac;                 /* sound heap */
extern BOOL data_0218fec0;                  /* sound system ready */

/* Frees what was loaded into the sound heap since the last saved state.
 * Level 0 (nothing saved) is left alone. */
void Sound_RestoreHeapState(void)
{
    int level;

    if (data_0218fec0 == 0) {
        return;
    }
    level = func_02106a6c(data_0218feac);
    if (level == 0) {
        return;
    }
    func_02106a78(data_0218feac, level);
}
