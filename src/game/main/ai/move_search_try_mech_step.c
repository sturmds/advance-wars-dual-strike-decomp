#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern int data_027c0750;
extern MoveSearchNode* data_027c0754;
extern MoveSearch* data_027c0758;
extern MoveSearchNode* data_027c075c;
extern int (*data_0229e174)(int x, int y, int unitType);

BOOL UnitType_CanEnterTerrain(int unitType, u8 terrain);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define DIR_UP 2
#define DIR_DOWN 3
#define DIR_LEFT 4
#define DIR_RIGHT 5

/* Steps from the node being expanded (data_027c0754) in `direction`. A tile
 * on the map that a Mech can enter, within maxCost steps, goes to the
 * callback in data_0229e174; if the callback does not accept it, it is added
 * to the list being collected (data_027c075c). TRUE when the callback accepts
 * the tile. */
BOOL MoveSearch_TryMechStep(int direction) {
  int steps;

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
  if (data_027c0758->x >= STATE->unk_1c4->mapWidth ||
      data_027c0758->y >= STATE->unk_1c4->mapHeight) {
    return FALSE;
  }
  if (!UnitType_CanEnterTerrain(UNIT_MECH,
                                MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(
                                    data_027c0758->x, data_027c0758->y)]))) {
    return FALSE;
  }
  steps = data_027c0754->cost + 1;
  if (steps > data_027c0758->maxCost) {
    return FALSE;
  }
  if (data_0229e174(data_027c0758->x, data_027c0758->y,
                    data_027c0758->unitType) == TRUE) {
    return TRUE;
  }
  data_027c075c->x = data_027c0758->x;
  data_027c075c->y = data_027c0758->y;
  data_027c075c->from = direction;
  data_027c075c->cost = steps;
  data_027c075c++;
  data_027c0750++;
  return FALSE;
}
