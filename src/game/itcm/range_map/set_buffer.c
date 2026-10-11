#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern MoveSearch *data_027c0758;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Points each rangeMap row at its row of tileMap, then stores the map size
 * in MoveSearch fields x and y (header comments call them the tile being
 * expanded; this code writes width and height there). */
void RangeMap_SetBuffer(void *tileMap)
{
    int row;

    for (row = 0; row < STATE->unk_1c4->mapHeight; row++) {
        STATE->rangeMap[row] = (s8 *)tileMap + STATE->mapTiles->rowStart[row];
    }
    data_027c0758->x = STATE->unk_1c4->mapWidth;
    data_027c0758->y = STATE->unk_1c4->mapHeight;
}
