#include "types.h"

void func_02108f50(void **handle, int fadeFrames);  /* NitroSystem NNS_SndArcStrmStop */

extern void *gStrmHandle;

/* Stops the stream started by Sound_StartStream, fading out over
 * fadeFrames frames. */
void Sound_StopStream(int fadeFrames)
{
    func_02108f50(&gStrmHandle, fadeFrames);
}
