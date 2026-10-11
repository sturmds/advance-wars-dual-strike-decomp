#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) (STATE->rangeMap[y][x])
#define TILE_INDEX(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define MARK 0x40

#define MARK_NEIGHBOUR(nx, ny)          \
    if (TILE(nx, ny) != MARK) {         \
        TILE(nx, ny)++;                 \
        TILE(nx, ny) |= MARK;           \
    }

/* Turns a move range into a direct attack range: every tile next to a
 * reachable tile (0 or more) the unit can stop on gets bit 0x40, and every
 * other tile becomes -1. A tile with a unit on it does not spread, except
 * the start tile (0). The caller passes 0x40 but the code does not read the
 * argument. */
void RangeMap_MarkDirectAttackRange(int unused)
{
    u8 x;
    u8 y;

    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            if (TILE(x, y) < 0 || TILE(x, y) == MARK) {
                continue;
            }
            if (TILE(x, y) != 0 && STATE->mapTiles->unitAt[TILE_INDEX(x, y)] != 0) {
                continue;
            }
            if (x != 0) {
                MARK_NEIGHBOUR(x - 1, y);
            }
            if (x != STATE->unk_1c4->mapWidth - 1) {
                MARK_NEIGHBOUR(x + 1, y);
            }
            if (y != 0) {
                MARK_NEIGHBOUR(x, y - 1);
            }
            if (y != STATE->unk_1c4->mapHeight - 1) {
                MARK_NEIGHBOUR(x, y + 1);
            }
        }
    }
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            if ((TILE(x, y) & MARK) == 0) {
                TILE(x, y) = -1;
            }
        }
    }
}
