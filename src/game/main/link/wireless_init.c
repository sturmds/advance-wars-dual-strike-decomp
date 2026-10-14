#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/wireless_parent_info.h"
#include "sdk/wm.h"
#include "game/Unk_0229c910.h"
#include "game/Unk_0229c960.h"
#include "game/link_player_name.h"
#include "game/wireless.h"
#include "game/Unk_0215fe88.h"

extern void* data_0229c884;
extern WMBssDesc* data_0229c888;
extern u32 gWirelessState;
extern void* data_0229c890;
WMScanParam* data_0229c8b4;
WMDataSet* data_0229c8b0;
WMDataSharingInfo* data_0229c8ac;
s32 data_0229c89c;
WirelessParentInfo* data_0229c8a0;
s32 data_0229c8a4;
WMBssDesc* gWirelessScanBuffer;
static void* gWirelessSystemBuf;
u16 gWirelessParentTimers[4];
Unk_0215fe88 data_0229c8c0;
WMBssDesc* gWirelessParents[4];
Unk_0229c910 data_0229c910;
u8 data_0229c8f8[WIRELESS_SSID_SIZE];
/* size: up to the next symbol; aligned to 32: no symbol covers the bytes before it */
__attribute__((aligned(32))) u8 data_0229c8e0[0x18];
Unk_0215fe84 data_0229c928;
Unk_0229c960 data_0229c960;
LinkPlayerName gLinkPlayerNames[0x8]; /* size: up to the next symbol */

void* Mem_Alloc(u32 size);
void Wireless_InitParentParam(void);

/* Resets the link state, allocates the wireless buffers (system buffer, parent
 * info, scan request, beacon buffers, data sharing, MP send and receive) and
 * starts the wireless driver on DMA channel 2. The MP send buffer is never
 * smaller than 0x60 bytes. */
void Wireless_Init(void) {
  int i;

  gWirelessState = 0;
  data_0215fe84->unk_08 = 0;
  data_0215fe84->unk_0c = 1;
  data_0215fe84->dataSharingActive = 0;
  data_0215fe84->scanning = 0;
  data_0215fe84->unk_30 = 0;
  data_0215fe84->unk_06 = -1;
  Wireless_InitParentParam();
  gWirelessSystemBuf = Mem_Alloc(WM_SYSTEM_BUF_SIZE);
  data_0229c8a0 = Mem_Alloc(sizeof(Unk_0229c960));
  data_0229c8b4 = Mem_Alloc(sizeof(WMScanParam));
  gWirelessScanBuffer = Mem_Alloc(sizeof(WMBssDesc));
  data_0229c888 = Mem_Alloc(sizeof(WMBssDesc));
  for (i = 0; i < 4; i++) {
    gWirelessParents[i] = Mem_Alloc(sizeof(WMBssDesc));
  }
  data_0229c8ac = Mem_Alloc(sizeof(WMDataSharingInfo));
  data_0229c8b0 = Mem_Alloc(sizeof(WMDataSet));
  data_0229c89c = WIRELESS_MP_SEND_SIZE;
  data_0229c884 = Mem_Alloc(WIRELESS_MP_SEND_SIZE);
  data_0229c8a4 = WIRELESS_MP_RECEIVE_SIZE;
  if (data_0229c89c < 0x60) {
    data_0229c89c = 0x60;
  }
  data_0229c890 = Mem_Alloc(WIRELESS_MP_RECEIVE_SIZE);
  WM_Init(gWirelessSystemBuf, 2);
}
