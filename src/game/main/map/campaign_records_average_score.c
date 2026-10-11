#include "types.h"
#include "game/Unk_02290118.h"

s32 HwDiv32(s32 numerator, s32 denominator);

/* Average score over the stored campaign map records (suggested name
 * CampaignRecords_GetAverageScore; callers outside this file keep the address
 * name). */
int func_020d618c(void)
{
    int totalScore = 0;
    int count = 0;
    int i;

    for (i = 0; i < 28 && data_022901a4[i].days != 0; i++) {
        totalScore += data_022901a4[i].score;
        count++;
    }
    return HwDiv32(totalScore, count);
}
