#include "types.h"
#include "game/Unk_ov004_022bf158.h"

typedef struct Unk_022baa9c Unk_022baa9c;

struct Unk_022baa9c {
  u8 pad_00[0x30];
  int unk_30;
};

extern Unk_ov004_022bf158* data_ov004_022bf158;

void func_ov004_022baa9c(Unk_022baa9c* a0) {
  int t0 = a0->unk_30;
  Unk_ov004_022bf158* t1 = data_ov004_022bf158;
  t1->slotState[t0] = 0;
}
