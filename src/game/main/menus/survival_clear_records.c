#include "types.h"
#include "game/play_records.h"
#include "game/Unk_022a7b80.h"
#include "game/survival_record.h"

extern Unk_022a7b80 data_022a7b80;

/* Clears the CO ids of the SURVIVAL_RECORD_COUNT record entries (the best time, funds and
 * days results) and the six saved runs, and puts the run back at its first
 * map. */
void Survival_ClearRecords(void) {
  int i;

  data_022a7b80.mapIndex = 0;
  for (i = 0; i < SURVIVAL_RECORD_COUNT; i++) {
    data_02291588[i].mode0Co = 0;
    data_02291588[i].mode2Co = 0;
    data_0229158c[i].mode1Co = 0;
  }
  for (i = 0; i < (int)ARRAY_COUNT(gPlayRecords.survivalRecords); i++) {
    gPlayRecords.survivalRecords[i].co = 0;
    gPlayRecords.survivalRecords[i].tagCo = 0;
    gPlayRecords.survivalRecords[i].value = 0;
  }
}
