#include "types.h"
#include "game/Unk_020226d4.h"

void SpriteAnim_Free(void *handle);

void func_02021bac(Unk_020226d4 *arg)
{
    SpriteAnim_Free(arg->unk_50);
}
