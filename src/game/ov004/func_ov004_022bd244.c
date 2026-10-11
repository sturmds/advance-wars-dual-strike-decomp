#include "types.h"

typedef struct Unk_022bd244 Unk_022bd244;

struct Unk_022bd244 {
    u8 pad_00[0x64];
    int unk_64;
};

void SpriteAnim_Free(int);

void func_ov004_022bd244(Unk_022bd244 *a0)
{
    SpriteAnim_Free(a0->unk_64);
}
