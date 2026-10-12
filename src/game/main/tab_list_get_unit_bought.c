#include "types.h"
#include "game/tab_list.h"

extern u32 data_02290a94[]; /* in gPlayRecords (+0x37c) */

/* Units of the entry's type bought by human players: CurrentPlayer_BuyUnit
 * counts them at record index 50 + type. */
int TabList_GetUnitBought(int row, int col) {
  return data_02290a94[data_0215d6c4[row][col].unk_04 + 50];
}
