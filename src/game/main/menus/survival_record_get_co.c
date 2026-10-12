#include "types.h"
#include "game/play_records.h"

/* CO id saved in the record of a Survival mode: the front CO for tag 0,
 * the second CO otherwise. */
int SurvivalRecord_GetCo(int mode, int tag) {
  if (tag == 0) {
    return gPlayRecords.survivalRecords[mode].co;
  }
  return gPlayRecords.survivalRecords[mode].tagCo;
}
