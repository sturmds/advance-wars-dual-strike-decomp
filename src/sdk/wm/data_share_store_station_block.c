#include "types.h"
#include "sdk/os.h"
#include "sdk/wm.h"

void MIi_CpuClear16(u16 data, void* dest, u32 size);
void MIi_CpuCopy16(const void* src, void* dest, u32 size);

/* Stores the block that station aid sent into the data set still waiting for
 * it (in double mode also the next one), or zeroes it when data is NULL, then
 * marks it received. */
void DataShare_StoreStationBlock(WMDataSharingInfo* dsInfo, u16 aid,
                                 u16* data) {
  u16 aidBit = 1 << aid;

  if (dsInfo->aidBitmap & aidBit) {
    u16* address;
    u16 writeIndex = dsInfo->writeIndex;
    int enabled;

    if ((dsInfo->ds[writeIndex].aidBitmap & aidBit) == 0) {
      if (dsInfo->doubleMode != TRUE) {
        return;
      }
      writeIndex = (writeIndex + 1) & (SHARE_RING_SLOTS - 1);
      if ((dsInfo->ds[writeIndex].aidBitmap & aidBit) == 0) {
        return;
      }
    }
    address = DataShare_StationBlockPtr(dsInfo, dsInfo->aidBitmap,
                                        dsInfo->ds[writeIndex].data, aid);
    if (data != NULL) {
      MIi_CpuCopy16(data, address, dsInfo->dataLength);
    } else {
      volatile u16 zero = 0; /* needed to match: the 0 goes through the stack */
      MIi_CpuClear16(zero, address, dsInfo->dataLength);
    }
    enabled = OS_DisableInterrupts();
    dsInfo->ds[writeIndex].aidBitmap &= ~aidBit;
    dsInfo->ds[writeIndex].receivedBitmap |= aidBit;
    OS_RestoreInterrupts(enabled);
  }
}
