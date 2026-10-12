#include "types.h"

typedef struct Unk_02021b20 Unk_02021b20;

struct Unk_02021b20 {
  u8 pad_00[0x22];
  u16 unk_22;
};

u16 func_02021b20(Unk_02021b20* a0) {
  u16 t0 = a0->unk_22;
  a0->unk_22 = 0;
  return t0;
}
