#include "types.h"

typedef struct Unk_020e36d8 Unk_020e36d8;

struct Unk_020e36d8 {
  u8 pad_00[0x40];
  int unk_40;
  int unk_44;
};

void func_020f5888(int a0, int a1);

void func_020e36d8(Unk_020e36d8* a0) {
  int t0 = a0->unk_40;
  func_020f5888((s16)t0, (s16)a0->unk_44);
}
