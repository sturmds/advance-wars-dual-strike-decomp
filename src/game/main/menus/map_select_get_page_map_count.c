#include "types.h"
#include "game/map_select.h"

/* Number of maps on the shown page of the current list: a full page of
 * ROWS * COLS maps, or what is left on the last page. */
int MapSelect_GetPageMapCount(void) {
  int left = LIST(TAB()).mapCount - PAGE() * (ROWS * COLS);

  if (left <= ROWS * COLS) {
    return left;
  }
  return ROWS * COLS;
}
