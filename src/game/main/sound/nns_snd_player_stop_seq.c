#include "types.h"
#include "game/sound.h"
#include "game/sound_handle.h"

void NNS_SndPlayerStopSeq(SoundHandle* handle, int fadeFrames) {
  SoundSeqPlayer_Stop(handle->player, fadeFrames);
}
