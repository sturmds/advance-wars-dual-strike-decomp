#include "types.h"

typedef struct Unk_02353204 Unk_02353204;
typedef struct Unk_02353204_1 Unk_02353204_1;

struct Unk_02353204 {
  u8 pad_00[0x2c];
  Unk_02353204_1* unk_2c;
};

struct Unk_02353204_1 {
  u8 pad_00[0x70];
  int unk_70;
};

void func_ov005_02353204(Unk_02353204* a0) { a0->unk_2c->unk_70 = 0; }
