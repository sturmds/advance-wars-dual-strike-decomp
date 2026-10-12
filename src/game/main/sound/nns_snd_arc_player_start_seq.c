#include "types.h"
#include "game/sound.h"
#include "game/sound_arc.h"

const SoundArcSeqInfo* NNS_SndArcGetSeqInfo(int seqNo);
BOOL SoundHandle_StartSeq(SoundHandle* handle, int playerNo, int bankNo,
                          int playerPrio, const SoundArcSeqInfo* info,
                          int seqNo);

/* NitroSystem NNS_SndArcPlayerStartSeq: starts sequence seqNo of the
 * current sound archive on handle. Returns TRUE if it started. */
BOOL NNS_SndArcPlayerStartSeq(SoundHandle* handle, int seqNo) {
  const SoundArcSeqInfo* info = NNS_SndArcGetSeqInfo(seqNo);

  if (info == NULL) {
    return FALSE;
  }
  return SoundHandle_StartSeq(handle, info->playerNo, info->bankNo,
                              info->playerPrio, info, seqNo);
}
