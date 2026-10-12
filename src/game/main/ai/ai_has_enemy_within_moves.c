#include "types.h"

BOOL func_020a5cc8(int x, int y, int unitType, int maxCost);
int Tile_HasEnemyUnit(int x, int y, int arg2);

/* Callback the path search calls for each tile. */
extern int (*data_0229e174)(int x, int y, int arg2);

/* TRUE when an enemy unit stands on tile (x, y), or the move search of
 * func_020a5cc8 for unitType and maxCost reaches one. */
BOOL Ai_HasEnemyWithinMoves(int x, int y, int unitType, int maxCost) {
  data_0229e174 = Tile_HasEnemyUnit;
  if (data_0229e174(x, y, unitType) == 1) {
    return TRUE;
  }
  return func_020a5cc8(x, y, unitType, maxCost);
}
