#include "types.h"
#include "sdk/wm.h"

/* The block station aid sent in receiveData, or NULL when that station is not
 * in the set or its block did not arrive. */
u16* WM_GetSharedDataAddress(WMDataSharingInfo* dsInfo, WMDataSet* receiveData,
                             u16 aid) {
  u16 aidBitmap = receiveData->aidBitmap;
  u16 receivedBitmap = receiveData->receivedBitmap;
  u32 aidBit = 1 << aid;

  if (dsInfo == NULL) {
    return NULL;
  }
  if (receiveData == NULL) {
    return NULL;
  }
  if ((aidBitmap & aidBit) == 0) {
    return NULL;
  }
  if ((receivedBitmap & aidBit) == 0) {
    return NULL;
  }
  return DataShare_StationBlockPtr(dsInfo, aidBitmap, receiveData->data, aid);
}
