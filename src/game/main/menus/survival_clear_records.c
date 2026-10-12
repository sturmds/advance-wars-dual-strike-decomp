#include "types.h"
#include "game/play_records.h"
#include "game/Unk_022a7b80.h"
#include "game/survival_record.h"

extern Unk_022a7b80 data_022a7b80;

/* Clears the CO ids of the 17 record entries (the best time, funds and
 * days results) and the six saved runs, and puts the run back at its first
 * map. */
void Survival_ClearRecords(void) {
  int i;

  data_022a7b80.mapIndex = 0;
  for (i = 0; i < 17; i++) {
    data_02291588[i].mode0Co = 0;
    data_02291588[i].mode2Co = 0;
    data_0229158c[i].mode1Co = 0;
  }
  for (i = 0; i < 6; i++) {
    gPlayRecords.survivalRecords[i].co = 0;
    gPlayRecords.survivalRecords[i].tagCo = 0;
    gPlayRecords.survivalRecords[i].value = 0;
  }
}
