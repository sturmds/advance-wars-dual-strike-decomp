#include "types.h"
#include "game/Unk_02290118.h"

s32 HwDiv32(s32 numerator, s32 denominator);

/* Average score over the stored campaign map records. */
int CampaignRecords_GetAverageScore(void) {
  int totalScore = 0;
  int count = 0;
  int i;

  for (i = 0; i < 28 && gCampaignMapRecords[i].days != 0; i++) {
    totalScore += gCampaignMapRecords[i].score;
    count++;
  }
  return HwDiv32(totalScore, count);
}
