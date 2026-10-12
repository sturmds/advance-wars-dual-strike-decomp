#include "types.h"

typedef struct Unk_020eef40 Unk_020eef40;

struct Unk_020eef40 {
  u8 pad_00[0x64];
  s16 unk_64;
  s16 unk_66;
};

void func_020f5888(int, int);

void func_020eef40(Unk_020eef40* a0) { func_020f5888(a0->unk_64, a0->unk_66); }
