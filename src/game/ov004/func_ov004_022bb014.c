#include "types.h"

typedef struct Unk_022bb014 Unk_022bb014;

struct Unk_022bb014 {
  u8 pad_00[0x828];
  u16 unk_828;
};

int func_ov004_022bb014(Unk_022bb014* a0, int a1) {
  a0->unk_828 = a1;
  return (int)a0 + 0x800;
}
