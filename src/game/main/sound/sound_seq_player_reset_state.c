#include "types.h"
#include "game/sound_handle.h"

void func_02109328(SoundFader* fader);
void SoundFader_Start(SoundFader* fader, int target, int frames);

/* Sets a sequence player to its start-of-sound state: no source, full
 * volume, fader at full volume. */
void SoundSeqPlayer_ResetState(SoundSeqPlayer* player) {
  player->unk_2e = 0;
  player->started = 0;
  player->sourceKind = SEQ_SOURCE_NONE;
  player->hwVolume = 0;
  player->seqVolume = 127;
  player->volume = 127;
  func_02109328(&player->volumeFader);
  SoundFader_Start(&player->volumeFader, 127 << 8, 1);
}
