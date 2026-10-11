#include "types.h"

/* The first argument is compared with the result of CTRDG_GetAgbMakerCode in
 * gba_cart_is_game.c, the second with CTRDG_GetAgbGameCode. */
BOOL GbaCart_IsGame(u32 first, u32 second);

/* TRUE when the GBA slot holds Advance Wars (0x3130 is "01", 0x45525741 is "AWRE"). */
BOOL GbaCart_IsAdvanceWars(void)
{
    return GbaCart_IsGame(0x3130, 0x45525741);
}
