#include "types.h"

int Ai_ScoreReachableEnemyProperties(void);
int Ai_CountTeamArmedGroundUnitsInRange(int);

int func_020b5198(void)
{
    int t0 = Ai_ScoreReachableEnemyProperties();
    int t1 = Ai_CountTeamArmedGroundUnitsInRange(t0);
    return t0 - t1;
}
