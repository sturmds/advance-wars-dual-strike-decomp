#include "types.h"
#include "game/Unk_0215fe84.h"

extern u8 data_0229c8e0[];

void MIi_CpuCopy16(const void* src, void* dst, u32 size);
void func_02078234(void);
int WM_SetMPDataToPortEx(void (*callback)(void), int arg, void* data, u32 size,
                         int destBitmap, int port, int priority);

/* Copies the data into data_0229c8e0 and queues it for all aids (bitmap 0xff)
 * on port 12, priority 2. True when queued; false sets Unk_0215fe84.unk_08. */
BOOL Wireless_SendMpData(const void* data, u32 size) {
  MIi_CpuCopy16(data, data_0229c8e0, size);
  if (WM_SetMPDataToPortEx(func_02078234, 0, data_0229c8e0, size, 0xff, 12,
                           2) != 2) {
    data_0215fe84->unk_08 = 1;
    return FALSE;
  }
  return TRUE;
}
