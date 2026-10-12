#include "types.h"
#include "game/play_records.h"

/* TRUE when a record is saved for the Survival mode (its CO id is not 0). */
BOOL SurvivalRecord_Exists(int mode) {
  return gPlayRecords.survivalRecords[mode].co != 0;
}
