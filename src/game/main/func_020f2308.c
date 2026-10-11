#include "types.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0 *data_021694c0;

int func_020f2308(void)
{
    Unk_021694c0 *scene = data_021694c0;
    return (scene->unk_3c == scene->unk_38) ? 1 : 0;
}
