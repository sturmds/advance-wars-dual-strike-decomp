#include "types.h"
#include "game/survival_record.h"
#include "game/Unk_022a7b80.h"

int Survival_GetRecordIndex(int mode, int index);

extern Unk_022a7b80 data_022a7b80;

/* TRUE when the saved record of the map at the list index, for the current
 * mode, has a CO stored (0 means no record). */
BOOL SurvivalSelect_HasRecord(int index) {
  switch (data_022a7b80.mode) {
    case 0:
    case 3:
      return data_02291588[Survival_GetRecordIndex(data_022a7b80.mode, index)]
                 .mode0Co != 0;
    case 2:
    case 5:
      return data_02291588[Survival_GetRecordIndex(data_022a7b80.mode, index)]
                 .mode2Co != 0;
    default:
      return data_0229158c[Survival_GetRecordIndex(data_022a7b80.mode, index)]
                 .mode1Co != 0;
  }
}
