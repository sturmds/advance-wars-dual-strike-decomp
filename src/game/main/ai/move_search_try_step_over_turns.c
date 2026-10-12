#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern int data_027c0750;
extern MoveSearchNode* data_027c0754;
extern MoveSearch* data_027c0758;
extern MoveSearchNode* data_027c075c;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(tiles, x, y) ((x) + (tiles)->rowStart[y])

/* STATE. Needed to match: here the original loads the index before the
 * base; a volatile read of the index is the only form found that keeps
 * that order. */
#define READ_STATE(dst)                          \
  {                                              \
    int index_ = *(volatile int*)&data_027c0284; \
    dst = data_027c027c->unk_48[index_];         \
  }

#define DIR_UP 2
#define DIR_DOWN 3
#define DIR_LEFT 4
#define DIR_RIGHT 5

/* The step of RangeMap_FillMovesOverTurns: moves from the node being
 * expanded one tile in direction. If the cost of the move is within
 * maxCost and beats what rangeMap holds for the tile, records it there,
 * records the turn count in MapTiles.rangeScratch, and queues the tile. */
void MoveSearch_TryStepOverTurns(int direction) {
  s8* rangeTile;
  MapTiles* tiles;
  int cost;
  int turns;
  int turnEnd;
  Unk_02080440_1c4* map;
  u8 x;
  unsigned long row;
  Unk_02080440_48* state;
  u8 y;
  u8 terrainCost;

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
    return;
  }
  READ_STATE(state);
  map = state->unk_1c4;
  x = data_027c0758->x;
  /* x and y are unsigned: a step off the top or left edge wraps to 255 */
  if (x >= map->mapWidth) {
    return;
  }
  y = data_027c0758->y;
  if (y >= map->mapHeight) {
    return;
  }
  tiles = state->mapTiles;
  /* Needed to match: an unsigned long copy of y, set here (u32 or a
     * cast does not match) */
  row = y;
  terrainCost =
      data_027c0758
          ->moveCost[MAP_TILE_TERRAIN(tiles->tiles[TILE(tiles, x, row)])];
  cost = data_027c0754->cost + terrainCost;
  if (cost > data_027c0758->maxCost) {
    return;
  }
  turns = tiles->rangeScratch[TILE(tiles, data_027c0754->x, data_027c0754->y)];
  /* a move that does not fit in the current turn starts the next one,
     * and the movement left over in this turn is lost */
  turnEnd = data_027c0758->moveRange * turns;
  if (turnEnd < cost) {
    cost = turnEnd + terrainCost;
    turns++;
  }
  rangeTile = &state->rangeMap[row][x];
  if (*rangeTile <= cost && *rangeTile >= 0) {
    return;
  }
  *rangeTile = cost;
  STATE->mapTiles->rangeScratch[TILE(STATE->mapTiles, data_027c0758->x,
                                     data_027c0758->y)] = turns;
  data_027c075c->x = data_027c0758->x;
  data_027c075c->y = data_027c0758->y;
  data_027c075c->from = direction;
  data_027c075c->cost = cost;
  data_027c075c++;
  data_027c0750++;
}
