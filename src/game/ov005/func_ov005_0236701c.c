#include "types.h"
#include "game/Unk_ov005_0236bbb0.h"

typedef struct Unk_0236701c {
  u8 pad_00[0x34];
  int unk_34;
} Unk_0236701c;

extern Unk_ov005_0236bbb0* data_ov005_0236bbb0;

void func_ov005_0236701c(Unk_0236701c* a0) {
  data_ov005_0236bbb0->transition = 2;
  a0->unk_34 = 0;
}
