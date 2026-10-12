#include "types.h"

typedef struct Unk_020d80c4_0 Unk_020d80c4_0;
typedef struct Unk_020d80c4 Unk_020d80c4;

struct Unk_020d80c4_0 {
  u8 pad_00[0x10];
  u8 unk_10;
};

struct Unk_020d80c4 {
  u8 pad_00[0x64];
  s16 unk_64;
};

Unk_020d80c4_0* EventScript_Start(void*);
extern u8 data_021677f0[];

void func_020d80c4(Unk_020d80c4* a0) {
  Unk_020d80c4_0* t0 = EventScript_Start(data_021677f0);
  t0->unk_10 = a0->unk_64;
}
