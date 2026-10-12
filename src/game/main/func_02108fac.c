#include "types.h"

typedef struct Unk_02108fac_0 Unk_02108fac_0;

struct Unk_02108fac_0 {
  u8 pad_00[0x10c];
  int unk_10c;
};

int func_02108fac(Unk_02108fac_0** a0) {
  Unk_02108fac_0* t0 = *a0;
  if ((int)t0 != 0) {
    int t1 = t0->unk_10c | 4;
    t0->unk_10c = t1;
    return t1;
  }
  return (int)a0;
}
