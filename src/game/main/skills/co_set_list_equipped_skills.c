#include "types.h"
#include "game/co_data.h"
#include "game/skills.h"

/* Writes the ids of the skills a CO has equipped in one set to `out`,
 * ended by 0xff. */
void CoSet_ListEquippedSkills(u8* out, int co, int set) {
  int skill;
  u32* equipped = gCoEquippedSkills[co][set];

  for (skill = 0; skill < SKILL_ID_COUNT; skill++) {
    if (CheckSkillsEquipped(equipped, skill)) {
      *out++ = skill;
    }
  }
  *out = 0xff;
}
