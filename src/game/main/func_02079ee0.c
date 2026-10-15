#include "types.h"
#include "game/Unk_0215fe84.h"

extern u32 data_0229c880;
void func_02079180(void*, int, int);

void func_02079ee0(void) {
  data_0215fe84->unk_2c = 1;
  data_0229c880 = 3;
  func_02079180(&data_0229c880, 3, 1);
}
