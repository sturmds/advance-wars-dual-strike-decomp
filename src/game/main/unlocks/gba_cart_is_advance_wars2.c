#include "types.h"

/* The first argument is compared with the result of CTRDG_GetAgbMakerCode in
 * gba_cart_is_game.c, the second with CTRDG_GetAgbGameCode. */
BOOL GbaCart_IsGame(u32 first, u32 second);

/* TRUE when the GBA slot holds Advance Wars 2 (0x3130 is "01", 0x45325741 is "AW2E"). */
BOOL GbaCart_IsAdvanceWars2(void) { return GbaCart_IsGame(0x3130, 0x45325741); }
