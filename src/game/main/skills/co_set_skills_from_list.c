#include "types.h"
#include "game/co_data.h"

extern u32 gCoEquippedSkills[CO_COUNT][4][4];

void Skills_Clear(u32* skills);
void PartnerBoost2(u32* skills, int id);

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
