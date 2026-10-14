#include "types.h"
#include "game/ov003_unit.h"
#include "game/ov003_marker.h"

extern Ov003Marker data_ov003_02369bb8[];

int Ov003Unit_CountInCurrentRow(void);

/* Returns 1 when the spawn row (not row 5) has used all its spawn records,
 * Ov003Unit_CountInCurrentRow returns 0 and no data_ov003_02369bb8 entry is busy. */
int Ov003_IsRowDone(void) {
  Ov003Marker* entry;
  int i;

  if (data_ov003_02364928->row == 5) {
    return 0;
  }
  if (data_ov003_02364928->nextSpawn <
      data_ov003_02364928->rows[data_ov003_02364928->row - 1].spawnCount) {
    return 0;
  }
  if (Ov003Unit_CountInCurrentRow() != 0) {
    return 0;
  }
  entry = data_ov003_02369bb8;
  for (i = 0; i < 4; i++, entry++) {
    if (entry->busy) {
      return 0;
    }
  }
  return 1;
}
