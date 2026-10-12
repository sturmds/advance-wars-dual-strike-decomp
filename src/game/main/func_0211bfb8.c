#include "types.h"

typedef struct Unk_022acee0 Unk_022acee0;

struct Unk_022acee0 {
  u8 pad_00[0x6];
  u16 unk_6;
};

extern Unk_022acee0 data_022acee0;

void func_0211bfb8(int a0) { data_022acee0.unk_6 = a0; }
