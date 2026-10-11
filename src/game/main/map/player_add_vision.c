#include "types.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u16 data_ov000_022f5c50[5];     /* per player slot: slot << 5, the owner bits of MapTiles.tiles */

void func_020c5540(MapUnit *unit, int a);
void func_020c5674(s16 x, s16 y, int range, int gridMask, int a, int b);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Adds what one player sees to the vision grids its Player.unk_1b bits pick:
 * the area around each of its units (only the first 51 of its 64 slots are
 * looked at) and every tile it owns. */
void Player_AddVision(int player)
{
    int i;
    int x;
    int y;

    if (STATE->players[player].controller == PLAYER_CONTROLLER_NONE) {
        return;
    }
    if (STATE->players[player].unk_1b == 0) {
        return;
    }
    for (i = 0; i < 51; i++) {
        func_020c5540(&STATE->units[Ov000_gPlayerFirstUnitSlot[player] + i], 1);
    }
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            if (data_ov000_022f5c50[player] == (STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]] & 0xe0)) {
                func_020c5674(x, y, 0, STATE->players[player].unk_1b, 1, 0);
            }
        }
    }
}
