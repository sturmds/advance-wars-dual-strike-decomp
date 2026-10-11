/* Needed to match: the library was built optimizing for size (no early return before the shared tail). */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/isd.h"
#include "sdk/os.h"

void Isd_BusSave(IsdBusState *saved)
{
    u16 exmemcnt = REG_EXMEMCNT;

    saved->exmemcnt = exmemcnt;
    REG_EXMEMCNT = (exmemcnt & 0xffe3) | 0xf;
    if (Isd_TestFlags(1)) {
        saved->active = 1;
        saved->sharedWord = ISD_SHARED_MODE;
        ISD_BUFFER_CONTROL = 0x20;
        ISD_SHARED_MODE = 0x20;
    }
}
