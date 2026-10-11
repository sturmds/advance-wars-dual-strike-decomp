#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) (STATE->rangeMap[y][x])

/* Marks the four tiles around (x, y) as reached (0), whatever they held.
 * The Repair and Supply menus call it for the acting unit's destination. */
void RangeMap_SetAdjacentReachable(int x, int y)
{
    if (x > 0) {
        TILE(x - 1, y) = 0;
    }
    if (x < STATE->unk_1c4->mapWidth - 1) {
        TILE(x + 1, y) = 0;
    }
    if (y > 0) {
        TILE(x, y - 1) = 0;
    }
    if (y < STATE->unk_1c4->mapHeight - 1) {
        TILE(x, y + 1) = 0;
    }
}
