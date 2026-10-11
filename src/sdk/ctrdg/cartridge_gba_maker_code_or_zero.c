#include "types.h"
#include "sdk/ctrdg.h"

u16 Cartridge_GbaMakerCodeOrZero(void)
{
    u16 makerCode = 0;
    GbaSlotBootSnapshot *info = GBA_SLOT_SNAPSHOT_PTR;

    if (Cartridge_HasGbaGame()) {
        makerCode = info->makerCode;
    }
    return makerCode;
}
