#include "types.h"
#include "game/sound.h"

int SoundArc_GetSeqArc(int itemNo, int loadFlags, void* heap, int cache,
                       void** loadedOut);

/* Loads a sequence archive into the sound heap. TRUE on success. */
BOOL NNS_SndArcLoadSeqArc(int itemNo, void* heap) {
  return SoundArc_GetSeqArc(itemNo, 0xff, heap, 1, NULL) == 0;
}
