#include "types.h"

typedef struct Unk_0207a7a4_0 Unk_0207a7a4_0;
typedef struct Unk_0229c960 Unk_0229c960;

struct Unk_0207a7a4_0 {
  u8 pad_00[0x35];
  u8 unk_35;
};

struct Unk_0229c960 {
  u8 pad_00[0x10];
  u16 unk_10;
};

extern Unk_0229c960 data_0229c960;
extern Unk_0207a7a4_0* data_0215fe84;

void func_0207a7a4(int a0, int a1) {
  data_0229c960.unk_10 = a1 - 1;
  data_0215fe84->unk_35 = a0;
}
