#include "types.h"
#include "game/Unk_0201d320.h"

extern Unk_0201d320 data_022977f4[];
void func_0201ce98(void);

/* Script command: calls func_0201ce98, then steps the slot's record pointer
 * on by one record (16 bytes). */
BOOL func_0201ce1c(s16 index)
{
    func_0201ce98();
    data_022977f4[index].unk_00++;
    return TRUE;
}
