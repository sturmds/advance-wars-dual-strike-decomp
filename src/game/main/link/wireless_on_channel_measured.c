#include "types.h"
#include "game/Unk_0215fe88.h"
#include "game/Unk_0229c960.h"
#include "sdk/wm.h"

extern Unk_0229c960* data_0229c8a0;
extern u32 data_0229c880;

BOOL Wireless_MeasureNextChannel(void);
void func_02079ee0(void);
BOOL func_0207909c(void);
void func_02079180(void);

/* Callback of the channel measurement. Keeps the quietest channel seen,
 * measures the next one, and when none is left starts the parent on the
 * best channel. */
void Wireless_OnChannelMeasured(WMMeasureChannelCallback* cb) {
  if (cb->errcode != 0) {
    data_0215fe88->bestChannel = 0;
  } else {
    if (data_0215fe88->bestBusyRatio > cb->ccaBusyRatio) {
      data_0215fe88->bestBusyRatio = cb->ccaBusyRatio;
      data_0215fe88->bestChannel = cb->channel;
    }
    if (Wireless_MeasureNextChannel()) {
      return;
    }
  }
  if (data_0215fe88->bestChannel == 0) {
    func_02079ee0();
  }
  data_0229c8a0->channel = data_0215fe88->bestChannel;
  if (func_0207909c()) {
    return;
  }
  if (data_0215fe88->mode == 1) {
    data_0229c880 = 0;
  }
  func_02079180();
}
