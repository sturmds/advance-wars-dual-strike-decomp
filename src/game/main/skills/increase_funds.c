#include "types.h"
#include "game/Unk_02080440.h"
#include "game/skills.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s8 Ov000_gIncomeTerrain
    [32]; /* nonzero for terrain that pays income, indexed by terrain id */

/* Skill words of a player's front CO. */
#define FRONT_CO_SKILLS(player) \
  (data_027c027c->unk_48[data_027c0284]->players[player].skills[0])

/* Income one tile of the given terrain pays a player, counting the
 * front CO's skills only. */
int IncreaseFunds(int playerIndex, int terrain) {
  int funds = 0;

  if (Ov000_gIncomeTerrain[terrain & 0x1f] != 0) {
    funds += data_027c027c->unk_48[data_027c0284]->fundsPerProperty;
    if (CheckSkillsEquipped(FRONT_CO_SKILLS(playerIndex),
                            SKILL_RANK_GOLD_RUSH)) {
      funds += 100;
    }
    if (CheckSkillsEquipped(FRONT_CO_SKILLS(playerIndex),
                            SKILL_PROPERTY_INCOME)) {
      funds += 100;
    }
  }
  return funds;
}
