#include "types.h"
#include "game/force_rank_screen.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate gForceRankSkillButtonTemplate;

/* Adds one button per listed skill, with the cursor (column << 3 | row) as
 * its id: columns 1-10 in 24-pixel steps from x 0 and 16-pixel rows from
 * y 56, and the "no skill" column alone at (240, 136). Then selects the
 * button at the saved cursor. Column 0 gets no buttons. */
void ForceRankScreen_AddButtons(void) {
  int column;
  int row;

  for (column = 1; column < FORCE_RANK_COLUMN_COUNT; column++) {
    if (column == FORCE_RANK_COLUMN_NONE) {
      MenuButton_Add(240, 136, &gForceRankSkillButtonTemplate, column << 3);
    } else {
      for (row = 0; row < gForceRankScreen->columns[column].count; row++) {
        MenuButton_Add((column - 1) * 24, (7 + row * 2) * 8,
                       &gForceRankSkillButtonTemplate, (column << 3) | row);
      }
    }
  }
  MenuButton_Select((gForceRankScreen->cursorColumn << 3) |
                    gForceRankScreen->cursorRow);
  MenuButtons_EnableTouch();
}
