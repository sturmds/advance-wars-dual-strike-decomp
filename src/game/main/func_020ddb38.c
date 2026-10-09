#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_action.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u32 gRngState;
extern u32 data_0229eaec;
extern UnitAction data_0229d8c8;

int func_020d29c0(void); /* weather for the next turn */
void Player_RefreshUnits(void);

#define GAME (data_027c027c->unk_48[data_027c0284])

void func_020ddb38(void)
{
    GAME->unk_1c4->nextWeather = func_020d29c0();
    data_0229d8c8.rngState = gRngState;
    if (data_0229eaec == 0) {
        GAME->players[GAME->currentPlayer].unk_31 = GAME->unk_28;
        GAME->players[GAME->currentPlayer].unk_32 = GAME->unk_2a;
    }
    GAME->unk_18 = 1;
    Player_RefreshUnits();
}
