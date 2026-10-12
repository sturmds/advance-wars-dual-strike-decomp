#include "types.h"

typedef struct Unk_037ff68c Unk_037ff68c;

struct Unk_037ff68c {
  u8 pad_00[0x48];
  int unk_48;
  int unk_4c;
};

void func_arm7_037ff68c(Unk_037ff68c* a0) {
  if ((int)a0 != 0) {
    a0->unk_48 = 0;
    a0->unk_4c = 0;
  }
}
