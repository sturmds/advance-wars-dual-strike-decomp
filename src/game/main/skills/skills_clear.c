#include "types.h"
#include "game/skills.h"

/* Turns off every skill in a set of four words; a NULL set is ignored. */
void Skills_Clear(u32* skills) {
  int i;

  if (skills == NULL) {
    return;
  }
  for (i = 0; i < SKILL_SET_WORDS; i++) {
    skills[i] = 0;
  }
}
