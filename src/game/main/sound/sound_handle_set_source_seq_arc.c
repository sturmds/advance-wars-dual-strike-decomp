#include "types.h"
#include "game/sound_handle.h"

/* Records on the handle's player that its sequence came from entry index
 * of sequence archive seqArcNo. Does nothing if the handle has no player. */
void SoundHandle_SetSourceSeqArc(SoundHandle* handle, int seqArcNo, int index) {
  if (handle->player == NULL) {
    return;
  }
  handle->player->sourceKind = SEQ_SOURCE_SEQ_ARC;
  handle->player->sourceNo = seqArcNo;
  handle->player->sourceIndex = index;
}
