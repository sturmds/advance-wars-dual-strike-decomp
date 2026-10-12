#include "types.h"

typedef struct Unk_020dc55c_0 Unk_020dc55c_0;
typedef struct Unk_020dc55c_1 Unk_020dc55c_1;

struct Unk_020dc55c_0 {
  u8 pad_00[0x48];
  Unk_020dc55c_1* unk_48[1];
};

struct Unk_020dc55c_1 {
  u8 pad_00[0x38];
  int unk_38;
};

extern Unk_020dc55c_0* data_027c027c;
extern int data_027c0284;
void func_ov000_022af35c(int, void*);

void func_020dc55c(void) {
  Unk_020dc55c_0* t0 = data_027c027c;
  int t1 = data_027c0284;
  func_ov000_022af35c(t0->unk_48[t1]->unk_38, t0);
}
