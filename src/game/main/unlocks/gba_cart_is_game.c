#include "types.h"
#include "sdk/ctrdg.h"

/* TRUE when the cartridge in the GBA slot has this maker code and game code,
 * both in header byte order (0x3130 is "01", 0x45525741 is "AWRE"). */
BOOL GbaCart_IsGame(u32 makerCode, u32 gameCode)
{
    u16 cartMakerCode;
    u32 cartGameCode;

    Cartridge_HeaderStillMatches();
    CTRDG_IsAgbCartridge();
    cartMakerCode = CTRDG_GetAgbMakerCode();
    cartGameCode = CTRDG_GetAgbGameCode();
    if (cartMakerCode == makerCode && cartGameCode == gameCode) {
        return TRUE;
    }
    return FALSE;
}
