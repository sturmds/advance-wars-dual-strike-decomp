#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern MapHeader Ov000_gMapHeaders[];

#define GAME (data_027c027c->unk_48[data_027c0284])
#define CURRENT_MAP_HEADER (Ov000_gMapHeaders[GAME->unk_1c4->mapId])

/* The game reads the header's army and team bytes with player numbers 1-4,
 * so index 0 is the byte before each array. */
#define HEADER_ARMY(player) ((&CURRENT_MAP_HEADER.unk_5c[0x2c])[player])
#define HEADER_TEAM(player) ((&CURRENT_MAP_HEADER.unk_8d)[player])

/* Copies each player's army and team, and the Fog of War flag, from the
 * current map's header into the game state. Only when unk_58 is 0, 1 or 3.
 * Callers pass the map id, but the function reads it from the state. */
void Map_LoadHeaderSetup(int mapId)
{
    int player;
    u8 mode;

    GAME->unk_1c4->unk_30 = 0;

    mode = GAME->unk_58;
    if (mode != 0 && mode != 1 && mode != 3) {
        return;
    }

    for (player = 1; player < 5; player++) {
        GAME->team[player] = HEADER_TEAM(player);
        GAME->army[player] = HEADER_ARMY(player);
    }

    if (CURRENT_MAP_HEADER.fog) {
        GAME->unk_1c4->fog = 1;
    }
}
