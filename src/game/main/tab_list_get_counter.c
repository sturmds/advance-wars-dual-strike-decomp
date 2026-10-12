#include "types.h"
#include "game/tab_list.h"

extern u32 data_02290a94[]; /* in gPlayRecords (+0x37c) */

/* Play record counter number unk_04 of the entry. */
int TabList_GetCounter(int row, int col) {
  return data_02290a94[data_0215d6c4[row][col].unk_04];
}
