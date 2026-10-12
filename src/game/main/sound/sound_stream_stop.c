#include "types.h"
#include "game/sound_stream.h"

void SoundStream_Close(SoundStream* stream);
void SoundFader_Start(SoundFader* fader, int target, int frames);

/* Stops a stream. With no fade frames, or when it never started playing, it
 * is closed at once; otherwise the volume fades to 0 and the stream closes
 * when the fade ends. */
void SoundStream_Stop(SoundStream* stream, int fadeFrames) {
  if (!stream->flags.playing) {
    SoundStream_Close(stream);
    return;
  }
  if (fadeFrames == 0) {
    SoundStream_Close(stream);
    return;
  }
  SoundFader_Start(&stream->volumeFader, 0, fadeFrames);
  stream->flags.stopping = 1;
  stream->priority = 0;
}
