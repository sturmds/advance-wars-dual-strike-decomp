#include "types.h"
#include "game/event_script.h"

void func_020f5888(s16 x, s16 y);
void func_0201d480(void);

BOOL func_0201d428(s16 index)
{
    func_020f5888(data_022977f0[index].pc->unk_08, data_022977f0[index].pc->unk_0a);
    data_022977f0[index].hold = (u32)func_0201d480;
    data_022977f0[index].pc++;
    return FALSE;
}
