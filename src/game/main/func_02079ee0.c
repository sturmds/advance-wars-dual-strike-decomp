#include "types.h"

typedef struct Unk_02079ee0_0 Unk_02079ee0_0;

struct Unk_02079ee0_0 {
  u8 pad_00[0x2c];
  int unk_2c;
};

extern Unk_02079ee0_0* data_0215fe84;
extern int data_0229c880;
void func_02079180(void*, int, int);

void func_02079ee0(void) {
  data_0215fe84->unk_2c = 1;
  data_0229c880 = 3;
  func_02079180(&data_0229c880, 3, 1);
}
