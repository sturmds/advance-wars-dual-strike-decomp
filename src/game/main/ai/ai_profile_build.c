#include "types.h"
#include "game/ai_profile.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

extern AiProfileMods gAiProfileMods[];

s32 Rng_Range(s32 min, s32 max);
/* Returns base + spread * roll / 100 - spread / 2, kept within 1-255. */
int AiProfile_Randomise(u8 base, u8 spread, int roll);

/* Fills the current player's AI profile from a template and a mods
 * entry. The template values given as base and spread are randomised
 * with one roll for the whole profile. */
void AiProfile_Build(AiProfile* profile, int modsId, int templateId) {
  AiProfileMods* mods = &gAiProfileMods[modsId];
  AiProfileTemplate* template = &gAiProfileTemplates[templateId];
  int roll;
  Player* player;
  int type;
  int i;
  u32 hasPartner; /* needed to match: as BOOL (int) the compare folds away */

  roll = Rng_Range(0, 100);
  player = &GAME->players[GAME->currentPlayer];
  if (player->aiBehaviour == AI_BEHAVIOUR_DEFENSE) {
    roll = 0;
  } else if (player->aiBehaviour == AI_BEHAVIOUR_ASSAULT) {
    roll = 100;
  } else if (player->tagPowerHalf == TAG_POWER_BACK) {
    roll = 100;
  }

  profile->unk_00 = template->unk_00;
  hasPartner = GAME->players[GAME->currentPlayer].back.id != 0;
  if (hasPartner == 1) {
    profile->unk_04 = template->unk_06;
    profile->unk_05 = template->unk_07;
  } else {
    profile->unk_04 = template->unk_04;
    profile->unk_05 = template->unk_05;
  }
  profile->unk_06 = template->unk_08;
  profile->unk_07 = template->unk_09;
  profile->unk_08[0] = mods->unk_00[0];
  profile->unk_08[1] = mods->unk_00[1];
  profile->unk_08[2] = mods->unk_00[2];
  profile->unk_08[3] = mods->unk_00[3];
  profile->unk_08[4] = mods->unk_00[4];
  profile->unk_08[5] = mods->unk_00[5];
  profile->unk_08[6] = mods->unk_00[6];
  profile->unk_08[7] = mods->unk_00[7];
  profile->unk_08[8] = mods->unk_00[8];
  profile->unk_08[9] = mods->unk_00[9];
  profile->unk_08[10] = mods->unk_00[10];
  profile->unk_08[11] = mods->unk_00[11];

  for (type = UNIT_INFANTRY; type < UNIT_TYPE_COUNT; type++) {
    profile->units[type - 1].unk_00 = mods->units[type - 1][0];
    profile->units[type - 1].unk_05 = mods->units[type - 1][1];
    profile->units[type - 1].threatMaxLimit =
        AiProfile_Randomise(template->units[type - 1].base1,
                            template->units[type - 1].spread1, roll);
    profile->units[type - 1].threatSumLimit =
        AiProfile_Randomise(template->units[type - 1].base2,
                            template->units[type - 1].spread2, roll);
    profile->units[type - 1].unk_03 = template->units[type - 1].unk_04;
    profile->units[type - 1].unk_04 = template->units[type - 1].unk_05;
    for (i = 0; i < 7; i++) {
      profile->units[type - 1].orderWeights[i] =
          template->units[type - 1].orderWeights[i];
    }
  }
}
