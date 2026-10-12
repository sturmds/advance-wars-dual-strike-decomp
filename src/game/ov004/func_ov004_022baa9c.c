#include "types.h"

typedef struct Unk_022baa9c_0 Unk_022baa9c_0;
typedef struct Unk_022baa9c Unk_022baa9c;

struct Unk_022baa9c_0 {
  u8 pad_00[0xa78];
  int unk_a78[1];
};

struct Unk_022baa9c {
  u8 pad_00[0x30];
  int unk_30;
};

extern Unk_022baa9c_0* data_ov004_022bf158;

void func_ov004_022baa9c(Unk_022baa9c* a0) {
  int t0 = a0->unk_30;
  Unk_022baa9c_0* t1 = data_ov004_022bf158;
  t1->unk_a78[t0] = 0;
}
