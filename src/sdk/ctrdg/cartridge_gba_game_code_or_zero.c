#include "types.h"
#include "sdk/ctrdg.h"

BOOL func_0211e594(void);

u32 Cartridge_GbaGameCodeOrZero(void)
{
    u32 gameCode = 0;
    GbaSlotBootSnapshot *info = GBA_SLOT_SNAPSHOT_PTR;

    if (func_0211e594()) {
        gameCode = info->gameCode;
    }
    return gameCode;
}
