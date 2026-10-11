#include "types.h"
#include "game/unit_stats.h"

BOOL func_020985b0(int mask);
int AiSite_PickPiperunnerSite(void);
int Ai_CountUnitsOfType(int type);

#define AI_PROFILE_PIPERUNNERS 1

BOOL AiBuild_WantsPiperunnerInstead(int unitType)
{
    if (!func_020985b0(AI_PROFILE_PIPERUNNERS)) {
        return FALSE;
    }
    if (AiSite_PickPiperunnerSite() == -1) {
        return FALSE;
    }
    return Ai_CountUnitsOfType(unitType) > Ai_CountUnitsOfType(UNIT_PIPERUNNER);
}
