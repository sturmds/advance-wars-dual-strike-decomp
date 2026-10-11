#include "types.h"

BOOL Player1Team_HasPlayerInGame(void);
void BattleRecord_SaveWin(void);
void BattleScore_Compute(void);
void func_020ea530(void);
void func_020ea608(void);
void PlayRecords_CountBattleRank(void);
void func_020eb0ec(void);

/* Runs the end-of-battle record updates (BattleScore_Compute, func_020ea608,
 * func_020ea530, PlayRecords_CountBattleRank), saves a win when player 1's team is still
 * in, then runs func_020eb0ec. */
void BattleEnd_UpdateRecords(void)
{
    BattleScore_Compute();
    func_020ea608();
    func_020ea530();
    PlayRecords_CountBattleRank();
    if (Player1Team_HasPlayerInGame()) {
        BattleRecord_SaveWin();
    }
    func_020eb0ec();
}
