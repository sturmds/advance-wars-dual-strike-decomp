#include "types.h"
#include "game/link_buffers.h"

extern Unk_0229ca88 data_0229ca88;

int Wireless_GetState(void);
BOOL Wireless_SendParentData(void* data, u32 size);
BOOL Wireless_SendChildData(void* data, u32 size);

/* Hands the oldest queued packet to the wireless layer as parent (state 4) or child (state 5).
 * The packet leaves the queue once the layer takes it; with any other state it is dropped. */
void Link_SendQueuedPacket(void) {
  LinkQueueSlot* slot;
  u8 readIndex = data_0229ca88.readIndex;

  if (readIndex == data_0229ca88.writeIndex) {
    return;
  }
  slot = &data_0229ca88.slots[readIndex];
  switch (Wireless_GetState()) {
    case 4:
      if (Wireless_SendParentData(slot, 24) == 0) {
        return;
      }
      break;
    case 5:
      if (Wireless_SendChildData(slot, 24) == 0) {
        return;
      }
      break;
  }
  data_0229ca88.readIndex++;
  if (data_0229ca88.readIndex == 8) {
    data_0229ca88.readIndex = 0;
  }
}
