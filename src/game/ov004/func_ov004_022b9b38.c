#include "types.h"

typedef struct Unk_022b9b38_0 Unk_022b9b38_0;

struct Unk_022b9b38_0 {
  u8 pad_00[0x8f4];
  int unk_8f4;
};

extern Unk_022b9b38_0* data_ov004_022bf158;

void func_ov004_022b9b38(void) { data_ov004_022bf158->unk_8f4 = 0; }
