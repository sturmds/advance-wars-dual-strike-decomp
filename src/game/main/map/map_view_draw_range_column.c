#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void MapView_DrawRangeTile(s16 x, s16 y);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Draws the range overlay for one column of tiles at mapX, starting at row
 * mapY: 13 rows, or 12 when the map ends within 12 rows. */
void MapView_DrawRangeColumn(int bufX, int bufY, s16 mapX, s16 mapY)
{
    u16 i;
    int count;

    count = (GAME->unk_1c4->mapHeight - mapY > 12) ? 13 : 12;
    for (i = 0; i < count; i++) {
        MapView_DrawRangeTile(mapX, mapY + i);
    }
}
