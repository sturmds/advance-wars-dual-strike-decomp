#include "types.h"
#include "game/skills.h"

void Skills_Clear(u32* skills);

/* Clears a skill set, then turns on the ids in `list` (up to 4, ended by 0). */
void Skills_SetFromList(const u8* list, u32* skills) {
  int i;

  Skills_Clear(skills);
  if (list == NULL) {
    return;
  }
  for (i = 0; i < SKILL_LIST_LENGTH; i++) {
    if (list[i] == 0) {
      return;
    }
    PartnerBoost2(skills, list[i]);
  }
}
