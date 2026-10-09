#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_020c4024(void);
void func_020c40d0(void);
void func_020f7268(int a);
void func_020cf884(int a);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Runs three calls only for a human current player; the purpose of the calls is not clear yet. */
void func_020800c0(void)
{
    if (STATE->players[STATE->currentPlayer].controller != PLAYER_CONTROLLER_AI) {
        func_020c4024();
        func_020c40d0();
        func_020f7268(4);
    }
    func_020cf884(3);
}
