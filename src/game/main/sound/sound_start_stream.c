#include "types.h"

/* NitroSystem NNS_SndArcStrmStart: returns TRUE if the stream started. */
BOOL func_02108f7c(void **handle, int strmNo, u32 offset);

extern void *gStrmHandle;

/* Starts sound archive stream strmNo from its beginning. */
void Sound_StartStream(int strmNo)
{
    func_02108f7c(&gStrmHandle, strmNo, 0);
}
