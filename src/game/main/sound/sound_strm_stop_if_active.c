#include "types.h"
#include "game/sound_strm.h"

void SoundStrm_Stop(SoundStrm* strm);

/* Stops and releases the stream if it was set up. */
void SoundStrm_StopIfActive(SoundStrm* strm) {
  if (strm->flags.active) {
    SoundStrm_Stop(strm);
  }
}
