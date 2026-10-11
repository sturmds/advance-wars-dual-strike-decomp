#include "types.h"
#include "sdk/ctrdg.h"

BOOL func_0211e594(void);

u16 Cartridge_GbaMakerCodeOrZero(void)
{
    u16 makerCode = 0;
    GbaSlotBootSnapshot *info = GBA_SLOT_SNAPSHOT_PTR;

    if (func_0211e594()) {
        makerCode = info->makerCode;
    }
    return makerCode;
}
