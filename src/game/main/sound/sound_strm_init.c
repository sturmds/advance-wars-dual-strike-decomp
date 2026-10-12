#include "types.h"
#include "game/sound_strm.h"

extern BOOL
    gSoundStrmListReady;       /* TRUE once the active stream list is set up */
extern LinkList data_022a892c; /* active streams */

void SoundStrm_OnSleep(SoundStrm* strm);
void SoundStrm_OnWake(SoundStrm* strm);

/* Sets up the stream list on first use, then puts a stream in its idle state:
 * sleep hooks pointing at the stream, no channels, not active. */
void SoundStrm_Init(SoundStrm* strm) {
  if (!gSoundStrmListReady) {
    LinkList_Init(&data_022a892c, 0);
    gSoundStrmListReady = TRUE;
  }
  strm->preSleep.callback = (void (*)(void*))SoundStrm_OnSleep;
  strm->preSleep.arg = strm;
  strm->postSleep.callback = (void (*)(void*))SoundStrm_OnWake;
  strm->postSleep.arg = strm;
  strm->channelMask = 0;
  strm->channelCount = 0;
  strm->flags.active = 0;
  strm->flags.running = 0;
}
