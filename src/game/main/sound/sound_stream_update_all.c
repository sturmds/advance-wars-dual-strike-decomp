#include "types.h"
#include "game/sound_stream.h"

extern s16 data_02151534[]; /* volume table */

void SoundStream_Close(SoundStream* stream);
void SoundStrm_Start(SoundStream* stream);
void SoundStrm_SetVolume(SoundStream* stream, int volume);
BOOL func_0210928c(SoundFader* fader);
void func_021092a4(SoundFader* fader);
int SoundFader_GetValue(SoundFader* fader);

/* Per-frame step for every stream slot: closes streams that finished,
 * starts the ones whose data is ready, and keeps the playing ones at the
 * volume the fader and the stream's own volume index give. */
void SoundStream_UpdateAll(void) {
  int i;
  int volume;
  int base;
  int fade;
  SoundStream* stream = data_022a9300;

  for (i = 0; i < SOUND_STREAM_COUNT; i++, stream++) {
    if (!stream->flags.fileOpen) {
      continue;
    }
    if (stream->unk_110 == 0) {
      SoundStream_Close(stream);
      continue;
    }
    if (stream->flags.startPending && stream->unk_114 != 0) {
      SoundStrm_Start(stream);
      stream->flags.playing = 1;
      stream->flags.startPending = 0;
    }
    if (!stream->flags.playing) {
      continue;
    }
    func_021092a4(&stream->volumeFader);
    base = data_02151534[stream->volumeIndex];
    fade = data_02151534[SoundFader_GetValue(&stream->volumeFader) >> 8];
    volume = fade + base;
    if (volume != stream->appliedVolume) {
      SoundStrm_SetVolume(stream, volume);
      stream->appliedVolume = volume;
    }
    if (stream->flags.stopping && func_0210928c(&stream->volumeFader)) {
      SoundStream_Close(stream);
    }
  }
}
