#include "types.h"
#include "game/Unk_02080440.h"
#include "game/skills.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Writes the ids of the Force Ranks (skill ids from SKILL_RANK_BRUISER up)
 * that a player's front (slot 0) or back (slot 1) CO has equipped to `out`,
 * ended by 0xff. */
void Player_ListEquippedSkills(u8* out, int player, int slot) {
  int skill;

  for (skill = SKILL_RANK_BRUISER; skill < 128; skill++) {
    if (CheckSkillsEquipped(
            data_027c027c->unk_48[data_027c0284]->players[player].skills[slot],
            skill)) {
      *out++ = skill;
    }
  }
  *out = 0xff;
}
