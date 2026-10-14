#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/Unk_0229c910.h"
#include "game/Unk_0229c960.h"

extern u8 data_0229c97a[];
extern u8 data_0229c982[];

int Wireless_GetBeaconJitter(void);
void MI_CpuFill8(void* dst, int value, int size);

/* Fills in the parent settings (the 0x40-byte WMParentParam layout):
 * game info buffer, game group id, up to 7 children, channel 1 and the
 * packet size limits. Wireless_Init calls it. */
void Wireless_InitParentParam(void) {
  data_0229c960.userGameInfo = &data_0229c910;
  data_0229c960.userGameInfoLength = 24;
  data_0229c960.unk_06 = 0;
  data_0229c960.ggid = 0x00400051;
  data_0229c960.tgid = 0;
  data_0229c960.entryFlag = 1;
  data_0229c960.unk_10 = 7;
  data_0229c960.multiBootFlag = 0;
  data_0229c960.ksFlag = 0;
  data_0229c960.csFlag = 0;
  data_0229c960.beaconPeriod = Wireless_GetBeaconJitter() + 180;
  MI_CpuFill8(data_0229c97a, 0, sizeof(data_0229c960.unk_1a));
  MI_CpuFill8(data_0229c982, 0, sizeof(data_0229c960.unk_22));
  data_0229c960.channel = 1;
  data_0229c960.parentMaxSize = 0x100;
  data_0229c960.childMaxSize = 0x40;
}
