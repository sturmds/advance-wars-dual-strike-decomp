#include "types.h"
#include "game/unit_target_entry.h"

extern UnitTargetEntry *data_022a6dcc;
extern int data_022a6dc8;

void func_020dc4c8(u16 a, u16 b, u16 unitSlot, u16 c)
{
    UnitTargetEntry *list = data_022a6dcc;

    list[data_022a6dc8].unk_04 = a;
    list[data_022a6dc8].unk_06 = b;
    list[data_022a6dc8].unitSlot = unitSlot;
    list[data_022a6dc8].unk_02 = c;
    data_022a6dc8++;
}
