#include "types.h"

typedef struct Unk_020f88b8_0 Unk_020f88b8_0;

struct Unk_020f88b8_0 {
  u8 pad_00[0xe4];
  int unk_e4;
};

extern Unk_020f88b8_0* data_027c0280;
void Mem_CopyAuto(void*, int, int, int);
extern u8 data_02139810[];

void func_020f88b8(void) {
  int t0 = data_027c0280->unk_e4;
  Mem_CopyAuto(data_02139810, t0 + 0x3ae0, 160, t0);
}
