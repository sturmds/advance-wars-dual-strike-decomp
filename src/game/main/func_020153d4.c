#include "types.h"

const void *Text_Get(u32 textId);
void Text_Print(int x, int y, u16 *map, const void *text, u16 unk_30, u16 unk_32);

void func_020153d4(int x, int y, u16 *map, u32 textId, u16 unk_30, u16 unk_32)
{
    const void *t0 = Text_Get(textId);
    Text_Print(x, y, map, t0, unk_30, unk_32);
}
