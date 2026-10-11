#include "types.h"

int func_020b6e4c(void);
int Ai_ScoreReachableEnemyProperties(void);
int Ai_CountTeamArmedGroundUnitsInRange(void);

/* After func_020b6e4c passes (name not known yet; func_020b5528 and others
 * also call it): TRUE when the value of the enemy properties the range map
 * reaches is above the number of the team's armed ground units it reaches. */
BOOL AiSeaTransport_PropertiesOutweighArmed(void)
{
    int properties;
    int armed;

    if (!func_020b6e4c()) {
        return FALSE;
    }
    properties = Ai_ScoreReachableEnemyProperties();
    armed = Ai_CountTeamArmedGroundUnitsInRange();
    return properties > armed;
}
