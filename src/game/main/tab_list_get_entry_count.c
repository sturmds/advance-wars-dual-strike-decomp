#include "types.h"
#include "game/tab_list.h"

/* Counts the entries of a tab, up to the first one marked as the end. */
int TabList_GetEntryCount(int tab) {
  int count = 0;
  TabListEntry* entry;

  if (data_0215d6c4[tab][0].unk_04 == TAB_LIST_END_MARK) {
    return 0;
  }
  entry = data_0215d6c4[tab];
  do {
    entry++;
    count++;
  } while (entry->unk_04 != TAB_LIST_END_MARK);
  return count;
}
