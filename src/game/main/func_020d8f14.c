#include "types.h"

typedef struct Unk_020d8f14 Unk_020d8f14;

struct Unk_020d8f14 {
  u8 pad_00[0x2c];
  int unk_2c;
  int unk_30;
};

void TileStrike_Start(int, int, int);

void func_020d8f14(Unk_020d8f14* a0) {
  TileStrike_Start(a0->unk_2c, a0->unk_30, 3);
}
