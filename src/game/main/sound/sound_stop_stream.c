#include "types.h"
#include "game/sound.h"

/* Stops the stream started by Sound_StartStream, fading out over
 * fadeFrames frames. */
void Sound_StopStream(int fadeFrames) {
  NNS_SndArcStrmStop(&gStrmHandle, fadeFrames);
}
