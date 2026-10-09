#include "types.h"

void func_02104e24(void);                   /* NitroSystem NNS_SndMain */

extern BOOL data_0218fec0;                  /* sound system ready */

/* Runs the NitroSystem sound update once the sound system is ready. */
void Sound_Update(void)
{
    if (data_0218fec0 == 0) {
        return;
    }
    func_02104e24();
}
