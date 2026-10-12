#include "types.h"

typedef struct Unk_0380388c_0 Unk_0380388c_0;

struct Unk_0380388c_0 {
  u8 pad_00[0x8];
  int unk_8;
};

int CardRom_IssueChipIdCommand(void);

int func_arm7_0380388c(Unk_0380388c_0** a0) {
  int t0 = CardRom_IssueChipIdCommand();
  Unk_0380388c_0* t1 = *a0;
  t1->unk_8 = t0;
  return t0;
}
