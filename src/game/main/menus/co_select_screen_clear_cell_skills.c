#include "types.h"
#include "game/co_select_screen.h"

/* Clears the skill list of every cell (3 columns, 2 rows). */
void CoSelectScreen_ClearCellSkills(void) {
  int i;
  int column;
  int row;

  for (column = 1; column <= 3; column++) {
    for (row = 0; row <= 1; row++) {
      for (i = 0; i < 4; i++) {
        data_0229c73c->cellSkills[column - 1][row][i] = 0;
      }
    }
  }
}
