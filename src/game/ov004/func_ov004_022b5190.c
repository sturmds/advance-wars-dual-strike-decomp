#include "types.h"

typedef struct Unk_022b5190 Unk_022b5190;

struct Unk_022b5190 {
  u8 pad_00[0x34];
  int unk_34;
  u8 pad_38[0xc];
  int unk_44;
  u8 pad_48[0xc];
  int unk_54;
  int unk_58;
};

void func_ov004_022b5190(Unk_022b5190* a0) {
  a0->unk_54 = 0;
  a0->unk_58 = -144;
  a0->unk_44 = 223;
  a0->unk_34 = 0;
}
