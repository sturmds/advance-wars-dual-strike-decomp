#include "types.h"
#include "game/skills.h"

/* Turns on skill id in a 4-word skill set; id 0 means none. The inverse
 * of CheckSkillsEquipped. */
void PartnerBoost2(u32* skills, int id) {
  if (id == 0) {
    return;
  }
  id--;
  skills[(id & 0xe0) >> 5] |= 1 << (id & 31);
}
