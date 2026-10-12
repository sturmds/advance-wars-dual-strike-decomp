#include "types.h"
#include "game/tab_list.h"

int TabList_CallUnk1c(int row, int col);

/* Counts the medals a player has earned and the medals there are: three per
 * entry in all four tabs. Entries whose unk_1c function returns nonzero add
 * to the total only. */
void TabList_CountMedals(u16* earnedOut, u16* totalOut) {
  u32 medal;
  int tab;
  int entry;
  u32 value;
  u16 earned;
  u16 total;
  earned = 0;
  total = 0;

  for (tab = 0; tab < 4; tab++) {
    for (entry = 0; data_0215d6c4[tab][entry].unk_04 != TAB_LIST_END_MARK;
         entry++) {
      value = data_0215d6c4[tab][entry].getValue(tab, entry);
      if (!TabList_CallUnk1c(tab, entry)) {
        for (medal = 0; medal < 3; medal++) {
          if (value >= data_0215d6c4[tab][entry].medalThreshold[medal]) {
            earned++;
          }
        }
      }
      total += 3;
    }
  }
  *earnedOut = earned;
  *totalOut = total;
}
