#include "types.h"

typedef struct Unk_020d2ed0_0 Unk_020d2ed0_0;

struct Unk_020d2ed0_0 {
  u8 pad_00[0x134];
  int unk_134;
};

extern Unk_020d2ed0_0* data_027c0280;
void File_UnpackTo(int, void*, int);
extern u8 data_02166e08[];

void func_020d2ed0(int a0) {
  int t0 = data_027c0280->unk_134;
  File_UnpackTo(t0 + (a0 << 5), data_02166e08, t0);
}
