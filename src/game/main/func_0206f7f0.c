#include "types.h"

u32 PlayerInfo_AddPageButtons(u32 co);
int PlayerInfo_AddCoButtons(int exp);

int func_0206f7f0(u32 co)
{
    return PlayerInfo_AddCoButtons(PlayerInfo_AddPageButtons(co));
}
