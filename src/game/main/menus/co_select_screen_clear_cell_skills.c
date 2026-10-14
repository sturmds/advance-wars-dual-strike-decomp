#include "types.h"
#include "game/force_rank_screen.h"
#include "game/co_select_screen.h"

/* Clears the skill list of every cell (3 columns, 2 rows). */
void CoSelectScreen_ClearCellSkills(void) {
  int i;
  int column;
  int row;

  for (column = 1; column <= (int)ARRAY_COUNT(data_0229c73c->cellSkills);
       column++) {
    /* Needed to match: the bound is written as <= count - 1. */
    for (row = 0; row <= (int)ARRAY_COUNT(data_0229c73c->cellSkills[0]) - 1;
         row++) {
      for (i = 0; i < FORCE_RANK_SLOT_COUNT; i++) {
        data_0229c73c->cellSkills[column - 1][row][i] = 0;
      }
    }
  }
}
