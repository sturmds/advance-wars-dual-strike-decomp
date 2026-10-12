#include "types.h"
#include "game/tab_list.h"

extern u32 data_02290a94[]; /* in gPlayRecords (+0x37c) */

/* A per-unit-type counter at record index 77 + the entry's unk_04. Unit_CountDestroyed
 * adds to it for a unit's type; what it counts is not known yet. */
int TabList_GetUnitStatB(int row, int col) {
  return data_02290a94[data_0215d6c4[row][col].unk_04 + 77];
}
