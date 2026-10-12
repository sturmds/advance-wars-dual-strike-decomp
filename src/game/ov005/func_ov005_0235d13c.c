#include "types.h"

typedef struct Unk_0235d13c_0 Unk_0235d13c_0;

struct Unk_0235d13c_0 {
  u8 pad_00[0x10];
  int unk_10;
};

extern Unk_0235d13c_0* data_ov005_0236bba0;

void func_ov005_0235d13c(void) { data_ov005_0236bba0->unk_10 = 1; }
