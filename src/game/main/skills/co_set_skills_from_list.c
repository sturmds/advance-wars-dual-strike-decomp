#include "types.h"
#include "game/co_data.h"
#include "game/skills.h"

void Skills_Clear(u32* skills);

/* Replaces the skills a CO has equipped in one set with the ids in `list`
 * (ended by 0xff). */
void CoSet_SetSkillsFromList(const u8* list, int co, int set) {
  u32* equipped = gCoEquippedSkills[co][set];

  Skills_Clear(equipped);
  if (*list == 0xff) {
    return;
  }
  do {
    PartnerBoost2(equipped, *list);
  } while (*++list != 0xff);
}
