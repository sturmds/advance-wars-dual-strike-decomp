#include "types.h"

typedef struct Unk_023634b0_0 Unk_023634b0_0;

struct Unk_023634b0_0 {
  u8 pad_00[0x48];
  s16 unk_48;
};

int func_ov005_023634b0(int a0, int a1) {
  return (((Unk_023634b0_0*)(a0 + (a1 << 4)))->unk_48 != 0) ? 1 : 0;
}
