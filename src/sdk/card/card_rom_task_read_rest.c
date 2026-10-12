#include "sdk/card.h"

BOOL CardRom_TakeFromBufferedPage(CARDiRomPage* page);
void CARDi_EndTask(void);

/* Card thread task for ROM reads. The common work argument is not used. */
void CardRom_TaskReadRest(CARDiCommon* common) {
  CARDiRomPage* page = &data_022ac100;

  if (CardRom_TakeFromBufferedPage(page)) {
    page->readPages(page);
  }
  CARDi_EndTask();
}
