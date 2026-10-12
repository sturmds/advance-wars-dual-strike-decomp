#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) (STATE->rangeMap[y][x])

/* Writes mark into each out-of-reach tile (negative) next to a reached tile
 * (0 or more). AiUnit_PickDirectAttack passes 111 after
 * RangeMap_FillUnitMoves, so mark stands for "attackable this turn".
 * Needed to match: `y != 0u` (an unsigned zero) on the row above; `y != 0`
 * gives other registers. Found by tools/permuter.py. */
void RangeMap_MarkAdjacentTiles(s8 mark)
{
    int x;
    int y;

    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            if (TILE(x, y) >= 0 && mark != TILE(x, y)) {
                if (x != 0 && TILE(x - 1, y) < 0) {
                    TILE(x - 1, y) = mark;
                }
                if (x != STATE->unk_1c4->mapWidth - 1 && TILE(x + 1, y) < 0) {
                    TILE(x + 1, y) = mark;
                }
                if (y != 0u && TILE(x, y - 1) < 0) {
                    TILE(x, y - 1) = mark;
                }
                if (y != STATE->unk_1c4->mapHeight - 1 && TILE(x, y + 1) < 0) {
                    TILE(x, y + 1) = mark;
                }
            }
        }
    }
}
