#include "types.h"

typedef struct Unk_022ace4c Unk_022ace4c;

struct Unk_022ace4c {
  u8 pad_00[0xd];
  u8 unk_d;
  u8 unk_e;
  u8 unk_f;
  u8 unk_10;
  u8 unk_11;
};

extern Unk_022ace4c data_022ace4c;

void* func_0211ba5c(void) {
  data_022ace4c.unk_d = 0;
  data_022ace4c.unk_e = 0;
  data_022ace4c.unk_f = 0;
  data_022ace4c.unk_10 = 0;
  data_022ace4c.unk_11 = 0;
  return &data_022ace4c;
}
