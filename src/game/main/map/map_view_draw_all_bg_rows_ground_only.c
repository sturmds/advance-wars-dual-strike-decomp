#include "types.h"
#include "game/screen_other_map.h"

void func_020fa748(u16 startCol, u16 row, s16 tileX, s16 tileY);

#define GAME (data_027c027c->unk_48[data_027c0284])
#define VIEW (GAME->unk_1c4)
#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Like MapView_DrawAllBgRows with func_020fa748 drawing each row, and only
 * the unk_0c hook afterwards. func_020fa748 writes only layer 3 (a guess
 * from its stores), hence the name. */
void MapView_DrawAllBgRowsGroundOnly(void)
{
    s16 i;

    for (i = 0; i < 13; i++) {
        func_020fa748((VIEW->scrollX >> 4) - VIEW->unk_26, i + ((VIEW->scrollY >> 4) - VIEW->unk_28),
                      VIEW->scrollX >> 4, i + (VIEW->scrollY >> 4));
    }
    if (HOOKS->unk_0c != NULL) {
        HOOKS->unk_0c();
    }
}
