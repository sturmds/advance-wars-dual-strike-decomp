#include "types.h"

typedef struct Unk_01ff9ac4_0 Unk_01ff9ac4_0;

struct Unk_01ff9ac4_0 {
  u8 pad_00[0x134];
  int unk_134;
};

void Mem_CopyAuto(void*, int, int);
extern u8 data_02139fd0[];
extern Unk_01ff9ac4_0* data_027c0280;

void func_01ff9ac4(void) {
  Mem_CopyAuto(data_02139fd0, data_027c0280->unk_134 + 0x6c00, 0x800);
}
