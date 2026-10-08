#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/skills.h"

BOOL func_020e8ae4(void);


extern int data_027c0284;
extern Unk_02080440 *data_027c027c;

/* Skill words of the front CO of a player. */
#define FRONT_CO_SKILLS(player) \
    (data_027c027c->unk_48[data_027c0284]->players[player].skills[0])

/* Vision bonus of a player: the front CO's bonus for its power state,
 * then the Scout and Eagle Eye Force Ranks. */
int VisionIncrease(int player)
{
    int bonus = 0;
    Unk_02080440_48 *game = data_027c027c->unk_48[data_027c0284];

    if (game->unk_5d) {
        Player *p = &game->players[player];
        /* needed to match: the CO id comes from a byte load and the
         * power state from shifts, not from the PLAYER_ macros */
        bonus += gCoTable[*(u8 *)&p->frontCo & PLAYER_CO_ID_MASK]
                     .blocks[(p->frontCo << 20) >> 30]
                     .visionBonus;
    }

    if (!func_020e8ae4()) {
        if (CheckSkillsEquipped(FRONT_CO_SKILLS(player), SKILL_RANK_SCOUT)) {
            bonus += 1;
        }
        if (CheckSkillsEquipped(FRONT_CO_SKILLS(player), SKILL_RANK_EAGLE_EYE)) {
            bonus += 2;
        }
    }
    return bonus;
}
