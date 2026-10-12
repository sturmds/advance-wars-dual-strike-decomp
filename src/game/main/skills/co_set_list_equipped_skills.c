#include "types.h"
#include "game/co_data.h"
#include "game/skills.h"

extern u32 gCoEquippedSkills[CO_COUNT][4][4];

/* Writes the ids of the skills a CO has equipped in one set to `out`,
 * ended by 0xff. */
void CoSet_ListEquippedSkills(u8* out, int co, int set) {
  int skill;
  u32* equipped = gCoEquippedSkills[co][set];

  for (skill = 0; skill < 128; skill++) {
    if (CheckSkillsEquipped(equipped, skill)) {
      *out++ = skill;
    }
  }
  *out = 0xff;
}
