#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) (GAME->rangeMap[y][x])

/* Writes value into every tile of the row and the column through (x, y),
 * except (x, y) itself. MapObject_Draw calls it for object kind 1. */
void RangeMap_FillRowAndColumn(int x, int y, s8 value)
{
    int i;

    for (i = 0; i < GAME->unk_1c4->mapWidth; i++) {
        if (i != x) {
            TILE(i, y) = value;
        }
    }
    for (i = 0; i < GAME->unk_1c4->mapHeight; i++) {
        if (i != y) {
            TILE(x, i) = value;
        }
    }
}
