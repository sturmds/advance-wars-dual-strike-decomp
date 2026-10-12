#include "types.h"
#include "game/force_rank_screen.h"

void ForceRankScreen_PutNoSkillIcon(int x,
                                    int y); /* puts the "no skill" icon */
void ForceRankScreen_PutSkillIcon(int x, int y,
                                  int skill); /* puts a skill's icon */

/* Puts the icons of every listed skill on the skills screen: columns 1-10 three
 * tiles apart, one icon every two rows, and the "no skill" entry at (30, 17). */
void ForceRankScreen_PutIcons(void) {
  int column;
  int i;

  for (column = 1; column < FORCE_RANK_COLUMN_COUNT; column++) {
    if (column == FORCE_RANK_COLUMN_NONE) {
      ForceRankScreen_PutNoSkillIcon(30, 17);
    } else {
      for (i = 0; i < gForceRankScreen->columns[column].count; i++) {
        ForceRankScreen_PutSkillIcon(
            (column - 1) * 3, 7 + i * 2,
            gForceRankScreen->columns[column].skills[i]);
      }
    }
  }
}
