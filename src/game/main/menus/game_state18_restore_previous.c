#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

void func_020ceb08(void);

/* Handler for game state 18 (see Game_EnterState18): goes back to the state
 * it replaced, which unk_1a holds. */
void Game_State18_RestorePrevious(void)
{
    STATE->unk_18 = STATE->unk_1a;
    STATE->unk_1a = 0;
    func_020ceb08();
}
