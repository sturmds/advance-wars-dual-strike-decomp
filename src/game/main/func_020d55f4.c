#include "types.h"

typedef struct Unk_020d55f4 Unk_020d55f4;

struct Unk_020d55f4 {
  u8 pad_00[0x54];
  int unk_54;
};

void Player_PlayTheme(int);

void func_020d55f4(Unk_020d55f4* a0) { Player_PlayTheme(a0->unk_54 & 255); }
