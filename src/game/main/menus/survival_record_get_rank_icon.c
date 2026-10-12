#include "types.h"
#include "game/play_records.h"

int Survival_GetGrade(int mode, u32 value);

/* Rank icon number of the saved record of a Survival mode: 0 for grade 5 (the
 * best), 1 for grade 4, 2 for grade 3 and 3 for the rest. */
int SurvivalRecord_GetRankIcon(int mode) {
  switch (Survival_GetGrade(mode & 0xff,
                            gPlayRecords.survivalRecords[mode].value)) {
    case 5:
      return 0;
    case 4:
      return 1;
    case 3:
      return 2;
    default:
      return 3;
  }
}
