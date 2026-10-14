#include "types.h"
#include "game/Unk_ov005_0236bba0.h"

typedef struct Unk_0235cee4 Unk_0235cee4;

struct Unk_0235cee4 {
  u8 pad_00[0x3];
  u8 unk_3;
};

extern Unk_ov005_0236bba0* data_ov005_0236bba0;

int func_ov005_0235cee4(Unk_0235cee4* a0) {
  u8 t0 = a0->unk_3;
  Unk_ov005_0236bba0* t1 = data_ov005_0236bba0;
  if ((u32)((t0 + 255) & 255) <= (u32)1) {
    t1->unk_82c = 0x1000;
    return 0x1000;
  }
  return (int)&data_ov005_0236bba0;
}
