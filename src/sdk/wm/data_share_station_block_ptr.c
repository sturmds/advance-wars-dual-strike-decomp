#include "types.h"
#include "sdk/math.h"
#include "sdk/wm.h"

/* Where the block of station aid starts in receiveBuf: blocks are packed in
 * aid order, one for each station in aidBitmap. */
u16 *DataShare_StationBlockPtr(WMDataSharingInfo *dsInfo, u16 aidBitmap, u16 *receiveBuf, u32 aid)
{
    u32 offset = MATH_CountPopulation(aidBitmap & ((1 << aid) - 1));

    return (u16 *)((u8 *)receiveBuf + offset * dsInfo->dataLength);
}
