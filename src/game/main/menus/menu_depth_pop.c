#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_020b9c68(void)
{
    Unk_02080440_48 *state = data_027c027c->unk_48[data_027c0284];

    if (state->menuDepth != 0) {
        state->menuDepth--;
    }
}
