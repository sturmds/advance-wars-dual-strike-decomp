#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440 *data_027c027c;

void func_02017524(u32 arg);

BOOL func_02016d24(int slot)
{
    func_02017524(data_027c027c->scriptSlots[slot].pc->arg);
    data_027c027c->scriptSlots[slot].pc++;
    return TRUE;
}
