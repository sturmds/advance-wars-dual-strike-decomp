#include "types.h"
#include "game/sound.h"
#include "game/sound_stream.h"

void SoundStream_Stop(SoundStream* stream, int fadeFrames);

/* NitroSystem NNS_SndArcStrmStop: stops the stream on handle, if any,
 * fading out over fadeFrames frames. */
void NNS_SndArcStrmStop(void** handle, int fadeFrames) {
  if (*handle == NULL) {
    return;
  }
  SoundStream_Stop(*handle, fadeFrames);
}
