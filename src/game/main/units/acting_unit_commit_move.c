#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_020dec70(void);
void func_020c8888(void);

/* Puts the acting unit on the tile its move ended on (unk_42, unk_44). */
void ActingUnit_CommitMove(void)
{
    func_020dec70();
    data_027c027c->unk_48[data_027c0284]->actingUnit->x = data_027c027c->unk_48[data_027c0284]->unk_42;
    data_027c027c->unk_48[data_027c0284]->actingUnit->y = data_027c027c->unk_48[data_027c0284]->unk_44;
    func_020c8888();
}
