#include "types.h"
#include "sdk/isd.h"

/* Retries until the cartridge bus is free. */
void Isd_AcquireCartridge(u32 *saved)
{
    while (!Isd_TryAcquireCartridge(saved)) {
    }
}
