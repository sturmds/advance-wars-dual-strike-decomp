#include "types.h"
#include "game/co_select_screen.h"

/* Tile row of grid row `row`: 2 * row with two rows, one lower with one. */
int CoSelectScreen_RowTileY(int row) {
  return (data_0229c73c->rowCount == 2) ? (row << 1) : ((row << 1) + 1);
}
