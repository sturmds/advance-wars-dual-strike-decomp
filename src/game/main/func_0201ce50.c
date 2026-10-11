#include "types.h"
#include "game/Unk_0201d320.h"

extern Unk_0201d320 data_022977f4[];
void func_0201ce84(void);

BOOL func_0201ce50(s16 index)
{
    func_0201ce84();
    data_022977f4[index].unk_00++;
    return TRUE;
}
