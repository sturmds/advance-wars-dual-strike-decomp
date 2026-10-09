#include "types.h"
#include "game/sound.h"

BOOL func_02107550(int itemNo, int loadFlags, void *heap, int arg3, void *arg4);

/* Loads a bank and its wave archives into the sound heap. TRUE on
 * success. func_02107550 starts from NNS_SndArcGetBankInfo. */
BOOL NNS_SndArcLoadBank(int itemNo, void *heap)
{
    return func_02107550(itemNo, 0xff, heap, 1, NULL) == 0;
}
