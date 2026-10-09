#include "types.h"

int func_02106b50(void *heap);              /* NitroSystem NNS_SndHeapSaveState */

extern void *data_0218feac;                 /* sound heap */
extern BOOL data_0218fec0;                  /* sound system ready */

/* Saves the sound heap state, so Sound_RestoreHeapState can later free
 * everything loaded after this point. */
void Sound_SaveHeapState(void)
{
    if (data_0218fec0 == 0) {
        return;
    }
    func_02106b50(data_0218feac);
}
