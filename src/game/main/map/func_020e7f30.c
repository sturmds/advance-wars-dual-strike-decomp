#include "types.h"
#include "game/co_data.h"
#include "game/map_header.h"

BOOL func_021039e4(void);

/* TRUE if func_021039e4 says so, or if the map lets any player pick a CO. */
BOOL func_020e7f30(int mapId)
{
    int player;
    int slot;

    if (func_021039e4() == TRUE) {
        return TRUE;
    }
    /* The game reads co[] with player numbers 1 to 4 (offsets 0x56 to 0x5d),
     * so co[4] runs into unk_5c. */
    for (player = 1; player < 5; player++) {
        for (slot = 0; slot < 2; slot++) {
            if (Ov000_gMapHeaders[mapId].co[player][slot] == CO_PLAYER_PICKS) {
                return TRUE;
            }
        }
    }
    return FALSE;
}
