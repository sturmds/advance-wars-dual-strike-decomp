#include "types.h"
#include "game/force_rank_screen.h"

void CoSet_SetSkillsFromList(const u8* list, int co, int set);
void ForceRank_SaveSets(int mode);

/* Stores the four chosen skills as the CO's equipped set, then saves all sets. */
void ForceRankScreen_SaveChosen(void) {
  CoSet_SetSkillsFromList(gForceRankScreen->chosenSkills, gForceRankScreen->co,
                          gForceRankScreen->set);
  ForceRank_SaveSets(gForceRankScreen->mode);
}
