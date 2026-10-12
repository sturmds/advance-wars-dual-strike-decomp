#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/Unk_0229c960.h"

void Wireless_SendMpData(void* data, u32 size);

/* Sends a packet from a child. A packet over the child size limit counts as
 * done (dropped); false means the link is not ready for it yet. */
BOOL Wireless_SendChildData(void* data, u32 size) {
  if (size > data_0229c960.childMaxSize) {
    return TRUE;
  }
  if (data_0215fe84->unk_08 == 0) {
    return FALSE;
  }
  data_0215fe84->unk_08 = 0;
  Wireless_SendMpData(data, size);
  return TRUE;
}
