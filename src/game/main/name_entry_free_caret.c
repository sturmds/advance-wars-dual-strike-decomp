#include "types.h"

typedef struct Unk_02100a58 Unk_02100a58;

struct Unk_02100a58 {
  u8 pad_00[0x4dc];
  int unk_4dc;
  int unk_4e0;
};

void SpriteAnim_Free(int);
int Mem_Free(int);

int NameEntry_FreeCaret(Unk_02100a58* a0) {
  SpriteAnim_Free(a0->unk_4e0);
  return Mem_Free(a0->unk_4dc);
}
