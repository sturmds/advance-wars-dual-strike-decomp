#include "types.h"

typedef struct Unk_02081214 Unk_02081214;
struct Unk_02081214 {
    u8 pad_00[0x30];
    int unk_30;
};
void SpriteAnim_Free(int);

void func_020ecb04(Unk_02081214 *a0)
{
    SpriteAnim_Free(a0->unk_30);
}
