#include "types.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"

#define STATE (data_027c027c->unk_48[data_027c0284])

void func_ov000_022b04ec(void);
BOOL Turn_NextPlayerWrapsAround(void);
BOOL Player_IsInGame(u8 player);
void LinkPlayer_MarkDefeated(int player, int code);
void Game_EnterState18(void);
void Game_EnterState20(void);

/* Handler for game state 1. When the map has a day limit (unk_80) and the
 * round ends on that day, every player in the game with fewer properties
 * (Player.propertyCount) than the most any player has is marked defeated (code 32)
 * and the game ends (state 20). Otherwise sets state 2 and enters state 18. */
void Game_State1_CheckDayLimit(void)
{
    int most;
    u8 player;

    if (data_027c027c == data_027c0288.unk_00) {
        func_ov000_022b04ec();
    }
    if (STATE->unk_80 == 0 || !Turn_NextPlayerWrapsAround() || STATE->unk_80 != STATE->day) {
        STATE->unk_18 = 2;
        Game_EnterState18();
        return;
    }
    most = 0;
    for (player = 1; player < 5; player++) {
        if (Player_IsInGame(player) && most < STATE->players[player].propertyCount) {
            most = STATE->players[player].propertyCount;
        }
    }
    for (player = 1; player < 5; player++) {
        if (Player_IsInGame(player) && most > STATE->players[player].propertyCount) {
            LinkPlayer_MarkDefeated(player, 32);
        }
    }
    Game_EnterState20();
}
