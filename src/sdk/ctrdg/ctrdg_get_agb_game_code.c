#include "types.h"
#include "sdk/ctrdg.h"

u32 CTRDG_GetAgbGameCode(void)
{
    u32 gameCode = 0;

    if (Cartridge_HeaderStillMatches()) {
        gameCode = Cartridge_GbaGameCodeOrZero();
    }
    return gameCode;
}
