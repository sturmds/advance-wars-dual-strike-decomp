#include "types.h"

typedef struct Unk_021062e8_0 Unk_021062e8_0;
typedef struct Unk_021062e8_1 Unk_021062e8_1;

struct Unk_021062e8_0 {
  u8 pad_00[0x14];
  int unk_14;
};

struct Unk_021062e8_1 {
  u8 pad_00[0x80];
  int unk_80;
};

/* also declared in src/game/main/func_021063d0.c and 14 more */
Unk_021062e8_1* gCurrentSoundArc;

void SoundArc_SetFileAddress(int a0, int a1) {
  ((Unk_021062e8_0*)(gCurrentSoundArc->unk_80 + (a0 << 4)))->unk_14 = a1;
}
