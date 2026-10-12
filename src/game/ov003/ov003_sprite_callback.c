#include "types.h"

typedef struct Unk_02364514 Unk_02364514;

struct Unk_02364514 {
  u8 pad_00[0x60];
  int unk_60;
};

void Ov003Unit_Remove(int);

void Ov003_SpriteCallback(Unk_02364514* a0) { Ov003Unit_Remove(a0->unk_60); }
