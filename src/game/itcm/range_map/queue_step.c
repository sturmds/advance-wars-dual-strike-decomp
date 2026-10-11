#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/range_map.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern int data_027c0750;
extern MoveSearchNode *data_027c0754;
extern MoveSearch *data_027c0758;
extern MoveSearchNode *data_027c075c;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* STATE, read index first. Needed to match: a volatile read of the index
 * keeps the original's load order (as in MoveSearch_TryStep). */
#define READ_STATE(dst)                                  \
    {                                                    \
        int index_ = *(volatile int *)&data_027c0284;    \
        dst = data_027c027c->unk_48[index_];             \
    }

/* One step of RangeMap_FillMoves from the node being expanded, by (x, y)
 * (one tile; x and y wrap past 0 to 255), recorded as from = direction.
 * The tile is queued and its range map entry set to the new cost when that
 * cost is lower than the entry (-1 counts as 255) and within maxCost. Here
 * moveRange holds RangeMap_FillMoves' last argument: when nonzero, tiles
 * with a unit of another team (unitType holds the moving team) block the
 * step. The search's x and y act as the bounds. */
void RangeMap_QueueStep(int direction, u8 x, u8 y)
{
    Unk_02080440_48 *state;
    MapTiles *tiles;
    int index;
    int unitSlot;
    s16 cost;

    x += data_027c0754->x;
    y += data_027c0754->y;
    if (x >= data_027c0758->x || y >= data_027c0758->y) {
        return;
    }
    READ_STATE(state);
    tiles = state->mapTiles;
    index = x + tiles->rowStart[y];
    cost = data_027c0758->moveCost[MAP_TILE_TERRAIN(tiles->tiles[index])]
        + state->rangeMap[data_027c0754->y][data_027c0754->x];
    if (cost >= (u8)state->rangeMap[y][x]) {
        return;
    }
    if (data_027c0758->moveRange != 0) {
        unitSlot = tiles->unitAt[index];
        if (unitSlot != 0 && data_027c0758->unitType != state->players[(unitSlot >> 6) + 1].team) {
            return;
        }
    }
    if (cost > data_027c0758->maxCost) {
        return;
    }
    if (data_027c0750 >= MOVE_SEARCH_LIST_SIZE - 1) {
        return;
    }
    data_027c0750++;
    data_027c075c->x = x;
    data_027c075c->y = y;
    data_027c075c->from = direction;
    data_027c075c->cost = cost;
    data_027c075c++;
    STATE->rangeMap[y][x] = cost;
}
