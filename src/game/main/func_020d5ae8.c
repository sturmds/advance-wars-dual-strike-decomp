#include "types.h"

typedef struct Unk_020d5ae8 Unk_020d5ae8;

struct Unk_020d5ae8 {
  u8 pad_00[0x29];
  u8 unk_29;
  u8 unk_2a;
  u8 pad_2b[0x1];
  u16 unk_2c;
};

void func_020d5b08(int, int, int);

void func_020d5ae8(Unk_020d5ae8* a0) {
  func_020d5b08(a0->unk_29, a0->unk_2a, a0->unk_2c + 192);
}
