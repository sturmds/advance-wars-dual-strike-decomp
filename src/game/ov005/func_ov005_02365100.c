#include "types.h"

typedef struct Unk_02365100 Unk_02365100;

struct Unk_02365100 {
  u8 pad_00[0xec];
  int unk_ec;
};

void Mem_Copy32(void*, int, int);
extern u8 data_ov005_0236b07c[];

void func_ov005_02365100(Unk_02365100* a0) {
  a0->unk_ec = (int)a0 + 172;
  Mem_Copy32(data_ov005_0236b07c, a0->unk_ec, 64);
}
