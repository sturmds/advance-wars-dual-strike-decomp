#include "types.h"
#include "game/force_rank_screen.h"
#include "game/co_select_screen.h"

BOOL Skills_AreDisabled(void);
void Co_ListDefaultSkills(u8* out, int co);

/* Rebuilds the skill list of one cell: clears it, and fills it from the
 * cell's CO unless skills are disabled. */
void CoSelectScreen_RefreshCellSkills(int column, int row) {
  int i;
  int left = column - 1;

  for (i = 0; i < FORCE_RANK_SLOT_COUNT; i++) {
    data_0229c73c->cellSkills[left][row][i] = 0;
  }
  if (Skills_AreDisabled() == 1) {
    return;
  }
  /* Needed to match: unsigned left and row in these two indexes (found by
     * tools/permuter.py). */
  Co_ListDefaultSkills(data_0229c73c->cellSkills[(u32)left][row],
                       (&data_0229c73c->coGrid[left * 2])[(u32)row]);
}
