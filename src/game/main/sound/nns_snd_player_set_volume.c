#include "types.h"
#include "game/sound.h"

/* NitroSystem NNS_SndPlayerSetVolume: sets the volume of the sequence
 * playing on handle, if any. */
void NNS_SndPlayerSetVolume(SoundHandle* handle, int volume) {
  if (handle->player != NULL) {
    handle->player->volume = volume;
  }
}
