#include "types.h"
#include "game/wireless.h"
#include "game/Unk_0215fe88.h"
#include "game/Unk_0229c960.h"
#include "sdk/wm.h"

extern Unk_0229c960* data_0229c8a0;
static u16 gWirelessTgidCounter;
u8 gWirelessParentLinkLevels[WIRELESS_PARENT_COUNT];
u32 data_0229c880;
void* data_0229c884;
WMBssDesc* data_0229c888;
u32 gWirelessState;
void* data_0229c890;

void MIi_CpuCopy32(const void* src, void* dst, u32 size);
BOOL Wireless_BeginChannelSurvey(void);
void func_02079ee0(void);

/* Copies our parent settings to the buffer the parent start request reads,
 * gives them a new tgid, and starts measuring the channels. If that cannot
 * start, falls back to func_02079ee0. */
void Wireless_StartChannelSurvey(int mode) {
  data_0215fe88->mode = mode;
  MIi_CpuCopy32(&data_0229c960, data_0229c8a0, sizeof(Unk_0229c960));
  gWirelessTgidCounter++;
  data_0229c8a0->tgid = gWirelessTgidCounter;
  if (!Wireless_BeginChannelSurvey()) {
    func_02079ee0();
  }
}
