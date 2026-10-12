#include "types.h"
#include "sdk/snd.h"

typedef struct Unk_037ff818 Unk_037ff818;

struct Unk_037ff818 {
  u8 pad_00[0x1e];
  u16 unk_1e;
};

int func_arm7_037ff818(Unk_037ff818* a0, int a1) {
  int t0 = SndExChannel_CalcDecayRate(a1);
  a0->unk_1e = t0;
  return t0;
}
