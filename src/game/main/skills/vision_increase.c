#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/skills.h"

BOOL Map_IsCoSelectPhase(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Skill words of the front CO of a player. */
#define FRONT_CO_SKILLS(player) \
  (data_027c027c->unk_48[data_027c0284]->players[player].skills[0])

/* Vision bonus of a player: the front CO's bonus for its power state,
 * then the Scout and Eagle Eye Force Ranks. */
int VisionIncrease(int player) {
  int bonus = 0;

  /* needed to match: no local for the battle state, or the two globals
     * load in the other order */
  if (data_027c027c->unk_48[data_027c0284]->coEffectsOn) {
    bonus +=
        gCoTable
            [data_027c027c->unk_48[data_027c0284]->players[player].front.id &
             PLAYER_CO_ID_MASK]
                .blocks[data_027c027c->unk_48[data_027c0284]
                            ->players[player]
                            .front.powerState]
                .visionBonus;
  }

  if (!Map_IsCoSelectPhase()) {
    if (CheckSkillsEquipped(FRONT_CO_SKILLS(player), SKILL_RANK_SCOUT)) {
      bonus += 1;
    }
    if (CheckSkillsEquipped(FRONT_CO_SKILLS(player), SKILL_RANK_EAGLE_EYE)) {
      bonus += 2;
    }
  }
  return bonus;
}
