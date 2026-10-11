#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(int player);
BOOL Player_HasHq(int player);
void func_020f5898(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* At battle start: when player 1 is in the game and has an HQ, puts the view
 * on the HQ tile (func_020f5898 centres it through func_020f53e8). */
void MapView_CentreOnPlayer1Hq(void)
{
    if (!Player_IsInGame(1)) {
        return;
    }
    if (!Player_HasHq(1)) {
        return;
    }
    func_020f5898(STATE->players[1].hqX & 0x7f, STATE->players[1].hqY & 0x7f);
}
