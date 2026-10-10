#include "types.h"

BOOL Player_IsInGame(u8 player);
int func_020cf17c(u16 player);
int func_020ceffc(int player);
int func_020cf090(u16 player);
int func_020cf0e4(u16 player);
void func_020cf2c8(int player, int code);

void LinkPlayers_RecordFirstFailure(void)
{
    s16 player;

    for (player = 1; player < 5; player++) {
        if (!Player_IsInGame(player)) {
            continue;
        }
        if (!func_020cf17c(player)) {
            func_020cf2c8(player, 8);
        } else if (!func_020ceffc(player)) {
            func_020cf2c8(player, 16);
        } else if (!func_020cf090(player)) {
            func_020cf2c8(player, 1);
        } else if (!func_020cf0e4(player)) {
            func_020cf2c8(player, 2);
        }
    }
}
