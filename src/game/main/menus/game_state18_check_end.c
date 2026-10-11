#include "types.h"

BOOL Players_IsOneTeamLeft(void);
void LinkPlayers_RecordFirstFailure(void);
void Game_EnterState20(void);
int Players_FindPropertyGoalWinner(void);
void func_02019a6c(int player, int arg);
void func_020cef2c(void);

/* Last step of Game_State18_RestorePrevious. When only one team still
 * stands (Players_IsOneTeamLeft), the failed checks are recorded and state
 * 20 is entered. Otherwise a
 * property goal winner is passed on to func_02019a6c with code 32, and with
 * no winner the link players are checked (func_020cef2c). */
void Game_State18_CheckEnd(void)
{
    int winner;

    if (Players_IsOneTeamLeft()) {
        LinkPlayers_RecordFirstFailure();
        Game_EnterState20();
        return;
    }
    winner = Players_FindPropertyGoalWinner();
    if (winner) {
        func_02019a6c(winner, 32);
        return;
    }
    func_020cef2c();
}
