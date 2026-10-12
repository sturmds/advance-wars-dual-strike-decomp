#include "types.h"

typedef struct Unk_02362bf0_0 Unk_02362bf0_0;

struct Unk_02362bf0_0 {
  u8 pad_00[0x3e];
  s16 unk_3e;
};

s16 func_ov005_02362bf0(int a0, int a1) {
  return ((Unk_02362bf0_0*)(a0 + (a1 << 4)))->unk_3e;
}
