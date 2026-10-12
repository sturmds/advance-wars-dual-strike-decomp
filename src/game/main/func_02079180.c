#include "types.h"
#include "game/Unk_0215fe84.h"

extern u32 data_0229c880;
extern u32 gWirelessState;

int WM_Reset(void (*callback)(void));
void func_020791f8(void);

int func_02079180(void) {
  data_0215fe84->parentCount = 0;
  data_0215fe84->unk_30 = 0;
  if (data_0215fe84->unk_18 == 0) {
    data_0215fe84->unk_20 = 1;
    data_0229c880 = 0;
  }
  if (WM_Reset(func_020791f8) != 2) {
    gWirelessState = 1;
    return 0;
  }
  return 1;
}
