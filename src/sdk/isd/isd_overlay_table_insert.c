/* Needed to match: the library was built optimizing for size. */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/isd.h"

static const char gIsdInsertIndexCheck[39] = {
#include "data/main/gIsdInsertIndexCheck.inc.c"
};

/* Opens a free entry at index in the overlay table, moving the later entries up. */
void Isd_OverlayTableInsert(IsdOverlayInfo* table, int index) {
  int i;

  if (index > table->header.infoCount) {
    Isd_ReportFailure(gIsdOverlayFileName, 81, gIsdInsertIndexCheck, TRUE);
  }
  /* Needed to match: the original tests count - index, not count != index. */
  if (table->header.infoCount - index != 0) {
    for (i = table->header.infoCount - 1; i >= index; i--) {
      table->info[i + 1] = table->info[i];
    }
  }
  table->header.infoCount++;
}
