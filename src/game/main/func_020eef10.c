#include "types.h"

typedef struct Unk_020eef10 Unk_020eef10;

struct Unk_020eef10 {
    u8 pad_00[0x64];
    s16 unk_64;
    s16 unk_66;
};

void SiloMissile_Launch(int, int, void *);

void func_020eef10(Unk_020eef10 *a0)
{
    SiloMissile_Launch(a0->unk_64, a0->unk_66, a0);
}
