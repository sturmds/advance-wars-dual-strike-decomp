#include "types.h"

typedef struct Unk_020e4e98 Unk_020e4e98;

struct Unk_020e4e98 {
  u8 pad_00[0x66];
  s16 unk_66;
  s16 unk_68;
};

void ExMachinaBolt_Start(int, int, void*);

void func_020e4e98(Unk_020e4e98* a0) {
  ExMachinaBolt_Start(a0->unk_66, a0->unk_68, a0);
}
