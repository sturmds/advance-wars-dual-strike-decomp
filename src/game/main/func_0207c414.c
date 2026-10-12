#include "types.h"

typedef struct Unk_0207c414 Unk_0207c414;

struct Unk_0207c414 {
  u8 pad_00[0x2c];
  int unk_2c;
  int unk_30;
  int unk_34;
  int unk_38;
};

void MenuCursor_DrawAt(int, int, int, int);

void func_0207c414(Unk_0207c414* a0) {
  MenuCursor_DrawAt(a0->unk_2c, a0->unk_30, a0->unk_34, a0->unk_38);
}
