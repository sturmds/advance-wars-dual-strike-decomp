#include "types.h"

typedef struct Unk_023659b8_0 Unk_023659b8_0;

struct Unk_023659b8_0 {
  u8 pad_00[0x8];
  int unk_8;
  u8 pad_0c[0x18];
  int unk_24;
};

extern Unk_023659b8_0* data_ov005_0236bbac;

int func_ov005_023659b8(u8* a0) {
  u8 t0 = *a0;
  Unk_023659b8_0* t1 = data_ov005_0236bbac;
  int t2 = t0 - 128;
  t1->unk_8 = t2;
  t1->unk_24 = 1;
  return t2;
}
