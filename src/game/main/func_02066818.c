#include "types.h"
#include "game/Unk_02066818.h"

extern Unk_02066818 data_0229eb30;

BOOL HasTwoOrMoreTeams(void)
{
    int teamUsed[4];
    int i;
    int teamCount;

    for (i = 0; i < 4; i++) {
        teamUsed[i] = 0;
    }
    for (i = 0; i < data_0229eb30.unk_2e3; i++) {
        teamUsed[data_0229eb30.unk_2c8[i]] = 1;
    }
    teamCount = 0;
    for (i = 0; i < 4; i++) {
        teamCount += teamUsed[i];
    }
    return teamCount >= 2;
}
