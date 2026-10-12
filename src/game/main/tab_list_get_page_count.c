#include "types.h"

s32 HwDiv32(s32 numerator, s32 denominator);
int TabList_GetEntryCount(int tab);

/* Pages of a tab, nine entries each; an empty tab still has one page. */
int TabList_GetPageCount(int tab) {
  int lastPage = HwDiv32(TabList_GetEntryCount(tab) - 1, 9);

  if (lastPage < 0) {
    lastPage = 0;
  }
  return lastPage + 1;
}
