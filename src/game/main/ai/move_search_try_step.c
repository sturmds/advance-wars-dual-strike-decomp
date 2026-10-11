#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern int data_027c0750;
extern MoveSearchNode *data_027c0754;
extern MoveSearch *data_027c0758;
extern MoveSearchNode *data_027c075c;

/* Callback the search hands each tile it reaches; TRUE stops the search. */
extern BOOL (*data_0229e174)(int x, int y, int unitType);

/* data_027c027c->unk_48[data_027c0284]. Needed to match: the original
 * loads the index before the base; a volatile read of the index is the
 * only form found that keeps that order. */
#define READ_STATE(dst)                                  \
    {                                                    \
        int index_ = *(volatile int *)&data_027c0284;    \
        dst = data_027c027c->unk_48[index_];             \
    }

#define DIR_UP    2
#define DIR_DOWN  3
#define DIR_LEFT  4
#define DIR_RIGHT 5

/* One step of MoveSearch_FindTile: moves from the node being expanded one
 * tile in direction and, if the path so far is within maxCost and the tile
 * is on the map, asks the callback about it. TRUE when the callback
 * accepts the tile; otherwise the tile is queued with the path cost so far
 * plus its terrain's move cost (capped at 255). */
BOOL MoveSearch_TryStep(int direction)
{
    Unk_02080440_48 *state;
    Unk_02080440_1c4 *map;
    MapTiles *tiles;
    int cost;

    if (data_027c0754->cost > data_027c0758->maxCost) {
        return FALSE;
    }
    data_027c0758->x = data_027c0754->x;
    data_027c0758->y = data_027c0754->y;
    switch (direction) {
    case DIR_UP:
        data_027c0758->y--;
        break;
    case DIR_DOWN:
        data_027c0758->y++;
        break;
    case DIR_LEFT:
        data_027c0758->x--;
        break;
    case DIR_RIGHT:
        data_027c0758->x++;
        break;
    }
    if (data_027c0750 >= MOVE_SEARCH_LIST_SIZE - 1) {
        return FALSE;
    }
    READ_STATE(state);
    map = state->unk_1c4;
    /* x and y are unsigned: a step off the top or left edge wraps to 255 */
    if (data_027c0758->x >= map->mapWidth || data_027c0758->y >= map->mapHeight) {
        return FALSE;
    }
    tiles = state->mapTiles;
    cost = data_027c0754->cost
        + data_027c0758->moveCost[MAP_TILE_TERRAIN(tiles->tiles[data_027c0758->x + tiles->rowStart[data_027c0758->y]])];
    if (cost > 255) {
        cost = 255;
    }
    if (data_0229e174(data_027c0758->x, data_027c0758->y, data_027c0758->unitType) == TRUE) {
        return TRUE;
    }
    data_027c075c->x = data_027c0758->x;
    data_027c075c->y = data_027c0758->y;
    data_027c075c->from = direction;
    data_027c075c->cost = cost;
    data_027c075c++;
    data_027c0750++;
    return FALSE;
}
