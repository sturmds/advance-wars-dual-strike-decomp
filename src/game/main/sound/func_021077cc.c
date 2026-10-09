#include "types.h"
#include "game/sound.h"

BOOL func_02107550(int itemNo, int loadFlags, void *heap, int arg3, void *arg4);

/* Loads one sound archive item into the sound heap (probably
 * NNS_SndArcLoadBank). TRUE on success. */
BOOL func_021077cc(int itemNo, void *heap)
{
    return func_02107550(itemNo, 0xff, heap, 1, NULL) == 0;
}
