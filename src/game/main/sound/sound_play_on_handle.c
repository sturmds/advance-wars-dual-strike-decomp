#include "types.h"
#include "game/sound_handle.h"
#include "game/sound.h"

/* NitroSystem NNS_SndArcPlayerStartSeqArc and NNS_SndArcPlayerStartSeq:
 * return TRUE if the sequence started. */

/* Plays sound soundId on handle. The high 16 bits pick a sequence
 * archive and the low 16 bits a sequence in it; with the high bits 0,
 * the low bits are a plain sequence number. */
void Sound_PlayOnHandle(SoundHandle* handle, u32 soundId) {
  u32 seqArcNo;
  u32 index;

  if (gSoundReady == 0) {
    return;
  }
  seqArcNo = (soundId & 0xffff0000) >> 16;
  index = soundId & 0xffff;
  if (seqArcNo != 0) {
    NNS_SndArcPlayerStartSeqArc(handle, seqArcNo, index);
  } else {
    NNS_SndArcPlayerStartSeq(handle, index);
  }
}
