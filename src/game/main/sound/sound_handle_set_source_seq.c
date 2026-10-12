#include "types.h"
#include "game/sound_handle.h"

/* Records on the handle's player that its sequence is number seqNo of the
 * sound archive. Does nothing if the handle has no player. */
void SoundHandle_SetSourceSeq(SoundHandle* handle, int seqNo) {
  if (handle->player != NULL) {
    handle->player->sourceKind = SEQ_SOURCE_SEQ;
    handle->player->sourceNo = seqNo;
  }
}
