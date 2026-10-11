#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_020d174c(void);
void UnitMoveSelect_Update(void);
void func_020d14cc(void);
void func_020d09a8(void);
void func_020d091c(void);
void func_020d08ac(void);
void func_020d1440(void);
void func_020d13ec(void);
void func_020d1360(void);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Game state 15 (the player's turn on the map) runs one handler per
 * sub-state, kept in unk_1c: 0 is the free cursor, 1 picks the move
 * destination (UnitMoveSelect_Update). The others are still unnamed. */
void Game_State15_RunStep(void)
{
    switch (GAME->unk_1c) {
    case 0:
        func_020d174c();
        break;
    case 1:
        UnitMoveSelect_Update();
        break;
    case 2:
        func_020d14cc();
        break;
    case 3:
        func_020d09a8();
        break;
    case 4:
        func_020d091c();
        break;
    case 5:
        func_020d08ac();
        break;
    case 6:
        func_020d1440();
        break;
    case 7:
        func_020d13ec();
        break;
    case 8:
        func_020d1360();
        break;
    }
}
