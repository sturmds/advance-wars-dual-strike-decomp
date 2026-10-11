#include "types.h"

BOOL Player1Team_HasPlayerInGame(void);
void BattleRecord_SaveWin(void);
void BattleScore_Compute(void);
void Campaign_AddCoScores(void);
void BattleResults_AwardExperience(void);
void PlayRecords_CountBattleRank(void);
void Survival_RecordMapResult(void);

/* Runs the end-of-battle record updates (BattleScore_Compute, BattleResults_AwardExperience,
 * Campaign_AddCoScores, PlayRecords_CountBattleRank), saves a win when player 1's team is still
 * in, then runs Survival_RecordMapResult. */
void BattleEnd_UpdateRecords(void)
{
    BattleScore_Compute();
    BattleResults_AwardExperience();
    Campaign_AddCoScores();
    PlayRecords_CountBattleRank();
    if (Player1Team_HasPlayerInGame()) {
        BattleRecord_SaveWin();
    }
    Survival_RecordMapResult();
}
