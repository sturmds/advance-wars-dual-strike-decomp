#include "types.h"
#include "game/skills.h"

/* Zeroes a saved list of four skill ids (the u8 form Skills_SetFromList reads); NULL is ignored. */
void ForceRank_ClearSet(u8* set) {
  int i;

  if (set == NULL) {
    return;
  }
  for (i = 0; i < SKILL_LIST_LENGTH; i++) {
    set[i] = 0;
  }
}
