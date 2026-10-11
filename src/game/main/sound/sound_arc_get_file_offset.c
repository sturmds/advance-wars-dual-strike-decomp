#include "types.h"

typedef struct Unk_021063f8_0 Unk_021063f8_0;
typedef struct Unk_021063f8_1 Unk_021063f8_1;
typedef struct Unk_021063f8_2 Unk_021063f8_2;

struct Unk_021063f8_0 {
    u8 pad_00[0x8];
    int unk_8;
};

struct Unk_021063f8_1 {
    u8 pad_00[0xc];
    int unk_c;
};

struct Unk_021063f8_2 {
    u8 pad_00[0x80];
    Unk_021063f8_0 *unk_80;
};

extern Unk_021063f8_2 *gCurrentSoundArc;

int SoundArc_GetFileOffset(int a0)
{
    Unk_021063f8_0 *t0 = gCurrentSoundArc->unk_80;
    if ((u32)a0 < (u32)t0->unk_8) {
    }
    return ((u32)a0 >= (u32)t0->unk_8) ? 0 : ((Unk_021063f8_1 *)((int)t0 + (a0 << 4)))->unk_c;
}
