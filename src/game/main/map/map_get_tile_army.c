#include "types.h"
#include "game/map_header.h"
#include "game/terrain.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

u8 func_020e0120(int player);

/* Returns the army style (1 Orange Star ... 5 Black Hole, 0 none) a map
 * tile is drawn in: the owner's, unless the current map sets one style for
 * every tile but HQs. */
u8 Map_GetTileArmy(int tile)
{
    u8 army = Ov000_gMapHeaders[data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId].tileArmy;

    if (army == 0 || MAP_TILE_TERRAIN(tile) == TERRAIN_HQ) {
        if ((tile & 0xe0) == 0) { /* needed to match: unshifted owner bits */
            army = 0;
        } else {
            army = func_020e0120(MAP_TILE_OWNER(tile));
        }
    }
    return army;
}
