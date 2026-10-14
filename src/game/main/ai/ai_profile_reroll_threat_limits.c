#include "types.h"
#include "game/ai_profile.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

int AiProfile_Randomise(u8 base, u8 spread, int roll);

/* Rolls the threat limits of every unit type again from a template. A Defense
 * player gets the low end of each range (roll 0), any other the high end
 * (roll 100). modsId is not used. */
void AiProfile_RerollThreatLimits(AiProfile* profile, int modsId,
                                  int templateId) {
  AiProfileTemplate* template = &gAiProfileTemplates[templateId];
  int roll;
  int type;

  roll = 100;
  if (GAME->players[GAME->currentPlayer].aiBehaviour == AI_BEHAVIOUR_DEFENSE) {
    roll = 0;
  }
  for (type = UNIT_INFANTRY; type < UNIT_TYPE_COUNT; type++) {
    profile->units[type - 1].threatMaxLimit =
        AiProfile_Randomise(template->units[type - 1].base1,
                            template->units[type - 1].spread1, roll);
    profile->units[type - 1].threatSumLimit =
        AiProfile_Randomise(template->units[type - 1].base2,
                            template->units[type - 1].spread2, roll);
  }
}
