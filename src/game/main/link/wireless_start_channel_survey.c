#include "types.h"
#include "game/Unk_0215fe88.h"
#include "game/Unk_0229c960.h"

extern Unk_0229c960* data_0229c8a0;
extern u16 gWirelessTgidCounter;

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
