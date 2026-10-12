#include "types.h"
#include "game/sound.h"

BOOL SoundArc_GetBank(int itemNo, int loadFlags, void* heap, int arg3,
                      void* arg4);

/* Loads a bank and its wave archives into the sound heap. TRUE on
 * success. SoundArc_GetBank starts from NNS_SndArcGetBankInfo. */
BOOL NNS_SndArcLoadBank(int itemNo, void* heap) {
  return SoundArc_GetBank(itemNo, 0xff, heap, 1, NULL) == 0;
}
