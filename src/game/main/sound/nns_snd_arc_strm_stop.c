#include "types.h"
#include "game/sound.h"

void func_021089f4(void *stream, int fadeFrames);

/* NitroSystem NNS_SndArcStrmStop: stops the stream on handle, if any,
 * fading out over fadeFrames frames. */
void NNS_SndArcStrmStop(void **handle, int fadeFrames)
{
    if (*handle == NULL) {
        return;
    }
    func_021089f4(*handle, fadeFrames);
}
