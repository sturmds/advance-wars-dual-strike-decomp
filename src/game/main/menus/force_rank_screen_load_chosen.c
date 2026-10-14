#include "types.h"
#include "game/force_rank_screen.h"

void CoSet_ListEquippedSkills(u8* list, int co, int set);
void ForceRank_LoadSets(int mode);

/* Loads the saved sets and fills chosenSkills with the CO's equipped skills.
 * The byte after the four slots is set to 0xff first (the list builder stops
 * there); the first 0xff left among the slots becomes 0 (empty). */
void ForceRankScreen_LoadChosen(void) {
  int i;

  ForceRank_LoadSets(gForceRankScreen->mode);
  for (i = 0; i < FORCE_RANK_SLOT_COUNT; i++) {
    gForceRankScreen->chosenSkills[i] = 0;
  }
  gForceRankScreen->chosenSkills[FORCE_RANK_SLOT_COUNT] = 0xff;
  CoSet_ListEquippedSkills(gForceRankScreen->chosenSkills, gForceRankScreen->co,
                           gForceRankScreen->set);
  for (i = 0; i < FORCE_RANK_SLOT_COUNT; i++) {
    if (gForceRankScreen->chosenSkills[i] == 0xff) {
      gForceRankScreen->chosenSkills[i] = 0;
      return;
    }
  }
}
