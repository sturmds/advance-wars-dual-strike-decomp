#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/skills.h"

#define PLAYER(i) (data_027c027c->unk_48[data_027c0284]->players[i])

/* TRUE when the front CO of a player strikes first when counterattacked:
 * Sonja's Super Power skill, or skill 0x49 while a Super Power is on. */
BOOL CounterFirst(int player) {
  if (CheckSkillsEquipped(PLAYER(player).skills[0], SKILL_COUNTER_FIRST) ||
      (PLAYER(player).front.powerState == POWER_SUPER &&
       CheckSkillsEquipped(PLAYER(player).skills[0], SKILL_RANK_MISTWALKER))) {
    return TRUE;
  }
  return FALSE;
}
