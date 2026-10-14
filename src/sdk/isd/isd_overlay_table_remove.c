/* Needed to match: the library was built optimizing for size. */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/isd.h"

static const char gIsdRemoveIndexCheck[] = {
#include "data/main/gIsdRemoveIndexCheck.inc.c"
};

/* Removes entry index from the overlay table, moving the later entries down. */
void Isd_OverlayTableRemove(IsdOverlayInfo* table, u32 index) {
  u32 count;

  if (index >= table->header.infoCount) {
    Isd_ReportFailure(gIsdOverlayFileName, 100, gIsdRemoveIndexCheck, TRUE);
  }
  table->header.infoCount--;
  count =
      table->header.infoCount; /* needed to match: read once, after the store */
  for (; index < count; index++) {
    table->info[index] = table->info[index + 1];
  }
}
