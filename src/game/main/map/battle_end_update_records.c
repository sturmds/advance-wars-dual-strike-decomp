#include "types.h"

BOOL Player1Team_HasPlayerInGame(void);
void BattleRecord_SaveWin(void);
void func_020ea240(void);
void func_020ea530(void);
void func_020ea608(void);
void func_020ea724(void);
void func_020eb0ec(void);

/* Runs the end-of-battle record updates (func_020ea240, func_020ea608,
 * func_020ea530, func_020ea724), saves a win when player 1's team is still
 * in, then runs func_020eb0ec. */
void BattleEnd_UpdateRecords(void)
{
    func_020ea240();
    func_020ea608();
    func_020ea530();
    func_020ea724();
    if (Player1Team_HasPlayerInGame()) {
        BattleRecord_SaveWin();
    }
    func_020eb0ec();
}
