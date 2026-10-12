#include "types.h"

typedef struct Unk_02101500_0 Unk_02101500_0;

struct Unk_02101500_0 {
  u8 pad_00[0x104];
  int unk_104;
  u8 pad_108[0x14];
  int unk_11c;
};

extern Unk_02101500_0* gNameEntry;

void func_02101500(u8* a0) {
  Unk_02101500_0* t0 = gNameEntry;
  t0->unk_11c = 1;
  t0->unk_104 = *a0 - 128;
}
