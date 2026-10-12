#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void MapView_DrawRangeTile(s16 x, s16 y);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Draws the range overlay for one row of 17 tiles at mapY, starting at column
 * mapX. */
void MapView_DrawRangeRow(int bufX, int bufY, s16 mapX, s16 mapY)
{
    u16 i;

    if (GAME->unk_1c4->mapHeight - mapY > 12) {
        for (i = 0; i < 17; i++) {
            MapView_DrawRangeTile(mapX + i, mapY);
        }
    }
}
