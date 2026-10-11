#include "types.h"

void Cartridge_HeaderStillMatches(void);
BOOL func_0211e5ac(void);
u32 func_0211e51c(void);    /* game code from the GBA cartridge header, 0 if none */
u16 func_0211e56c(void);    /* maker code from the GBA cartridge header, 0 if none */

/* TRUE when the cartridge in the GBA slot has this game code and maker
 * code, both in header byte order ("AWRE" reads as 0x45525741). */
BOOL GbaCart_IsGame(u32 gameCode, u16 makerCode)
{
    u32 cartGameCode;
    u16 cartMakerCode;

    Cartridge_HeaderStillMatches();
    func_0211e5ac();
    cartGameCode = func_0211e51c();
    cartMakerCode = func_0211e56c();
    if (cartGameCode == gameCode && cartMakerCode == makerCode) {
        return TRUE;
    }
    return FALSE;
}
