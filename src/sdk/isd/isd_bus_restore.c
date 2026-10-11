#include "types.h"
#include "sdk/isd.h"
#include "sdk/os.h"

void Isd_BusRestore(IsdBusState *saved)
{
    vu16 dummy;

    if (Isd_TestFlags(1) && saved->active != 0) {
        ISD_SHARED_MODE = saved->sharedWord;
        ISD_BUFFER_CONTROL = saved->sharedWord;
        dummy = ISD_BUFFER_STATUS;
        dummy = ISD_BUFFER_STATUS;
        dummy = ISD_BUFFER_STATUS;
    }
    REG_EXMEMCNT = saved->exmemcnt;
}
