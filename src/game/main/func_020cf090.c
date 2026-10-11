#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int func_020c07d8(int player);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* One of four per-player checks run by LinkPlayers_RecordFirstFailure (this one
 * records code 1). Passes unless bit 1 of unk_1c4->unk_37 is set; then
 * func_020c07d8 decides. */
int func_020cf090(u16 player)
{
    if (!(STATE->unk_1c4->unk_37 & 1)) {
        return 1;
    }
    return func_020c07d8(player);
}
