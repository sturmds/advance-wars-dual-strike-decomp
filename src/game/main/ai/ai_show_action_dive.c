#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_020ca550(int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Display step of action 11: the acting unit dives. */
void func_020b8308(void)
{
    func_020ca550(STATE->actingUnit->type);
    STATE->actingUnit->flags |= MAP_UNIT_DIVED;
}
