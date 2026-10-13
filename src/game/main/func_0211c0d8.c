#include "types.h"
#include "sdk/mb.h"

void MbParent_StartCore(int, int, int);

void func_0211c0d8(int a0) {
  DlPlayHostLinkArea* t0 = data_022aced4;
  t0->unk_1320 = 1;
  MbParent_StartCore(a0, (int)t0 + 0x1000, 1);
}
