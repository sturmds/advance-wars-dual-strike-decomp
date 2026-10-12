#include "types.h"

typedef struct Unk_022ade28 Unk_022ade28;

struct Unk_022ade28 {
  u8 pad_00[0x2c];
  int unk_2c;
};

void func_ov004_022bb9c4(void);
int Mem_Free(int);

int func_ov004_022ade28(Unk_022ade28* a0) {
  int t0 = a0->unk_2c;
  func_ov004_022bb9c4();
  return Mem_Free(t0);
}
