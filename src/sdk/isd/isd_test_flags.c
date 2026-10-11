#include "types.h"
#include "sdk/isd.h"

/* TRUE when any bit of mask is set in the shared flag word. The local pointer keeps the base and offset apart, as the original does. */
BOOL Isd_TestFlags(u32 mask)
{
    volatile IsdSharedBlock *block = ISD_SHARED;

    return (mask & block->flags) != 0;
}
