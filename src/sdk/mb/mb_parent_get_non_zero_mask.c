#include "types.h"
#include "sdk/mb.h"

extern DlPlayStationArea* data_022aced8;

/* Bit mask of the work area's four u16 values at 0x0e, 0x12, 0x14 and 0x16 that
 * are not zero (bits 0 to 3); what they count is not known yet. */
u8 MbParent_GetNonZeroMask(void) {
  DlPlayStationArea* work = data_022aced8;

  return (work->unk_0e != 0 ? 1 : 0) | (work->unk_12 != 0 ? 2 : 0) |
         (work->unk_14 != 0 ? 4 : 0) | (work->unk_16 != 0 ? 8 : 0);
}
