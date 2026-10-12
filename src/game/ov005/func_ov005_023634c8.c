#include "types.h"

typedef struct Unk_023634c8_0 Unk_023634c8_0;

struct Unk_023634c8_0 {
  u8 pad_00[0xc];
  int unk_c;
};

extern Unk_023634c8_0* data_ov005_0236bba8;

void func_ov005_023634c8(void) { data_ov005_0236bba8->unk_c = 1; }
