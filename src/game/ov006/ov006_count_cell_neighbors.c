#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern u32 data_027c0284;
extern s8 data_ov006_0236d744[][30];   /* per tile, 30 a row: nonzero marks a planned tile (2: road) */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CELL_AT(x, y) (STATE->mapTiles->mapCells[STATE->mapTiles->rowStart[y] + (x)])

/* Counts how many of the four neighbours of x, y hold map cell code `cell`;
 * a side off the map counts too. A tile marked in the plan grid gets 0. */
int Ov006_CountCellNeighbors(int x, int y, int cell)
{
    int count;

    if (data_ov006_0236d744[y][x] != 0) {
        return 0;
    }
    count = 0;
    if (x - 1 < 0 || cell == CELL_AT(x - 1, y)) {
        count++;
    }
    if (y - 1 < 0 || cell == CELL_AT(x, y - 1)) {
        count++;
    }
    if (x + 1 >= STATE->unk_1c4->mapWidth || cell == CELL_AT(x + 1, y)) {
        count++;
    }
    if (y + 1 >= STATE->unk_1c4->mapHeight || cell == CELL_AT(x, y + 1)) {
        count++;
    }
    return count;
}
