#include "types.h"

/* NitroSystem NNS_SndArcStrmStart: returns TRUE if the stream started. */
BOOL func_02108f7c(void **handle, int strmNo, u32 offset);

extern void *data_0218febc;                 /* stream handle */

/* Starts sound archive stream strmNo from its beginning. */
void Sound_StartStream(int strmNo)
{
    func_02108f7c(&data_0218febc, strmNo, 0);
}
