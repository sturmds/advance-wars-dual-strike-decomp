#include "types.h"
#include "game/battle_save.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/skills.h"
#include "game/screen_other_map.h"

u32* GetCoPowerSkills(int co, int powerState);
BOOL PartnerBoost1(int player);

extern Unk_02080440_48 data_02183b08;
extern BattleSaveSkillSet data_022a72c8
    []
    [PLAYER_CO_COUNT]; /* skills a tag partner lends, per player and CO slot */

/* Shorthand for player i of the current state; the original code reloads
 * it through the globals on every access. */
#define PLAYER(i) (data_027c027c->unk_48[data_027c0284]->players[i])

/* Rebuilds a player's two skill sets (front CO, back CO) from each CO's
 * skills in the front CO's power state, plus skills the tag partner lends
 * when PartnerBoost1 allows them. */
void UpdatePlayerSkills(int player) {
  int frontCo;
  int backCo;
  u32* frontSkills;
  u32* backSkills;
  int i;
  int slot;

  frontCo = PLAYER(player).front.id & PLAYER_CO_ID_MASK;
  backCo = PLAYER(player).back.id & PLAYER_CO_ID_MASK;
  /* Both COs use the front CO's power state. */
  frontSkills = GetCoPowerSkills(frontCo, PLAYER(player).front.powerState);
  backSkills = GetCoPowerSkills(backCo, PLAYER(player).front.powerState);

  for (slot = 0; slot < PLAYER_CO_COUNT; slot++) {
    for (i = 0; i < SKILL_SET_WORDS; i++) {
      PLAYER(player).skills[slot][i] = 0;
    }
  }

  if (data_027c027c->unk_48[data_027c0284]->secondMapId != 0) {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;
    u32 otherUnk30;

    /* data_027c0288 holds two states, indexed like data_027c0284;
         * switch to the other one to read its unk_30. */
    Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
    saved027c = data_027c027c;
    Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
    otherUnk30 = data_027c027c->unk_48[data_027c0284]->unk_1c4->unk_30;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    if (otherUnk30 == 0) {
      /* Only the front CO gets skills; the lent set depends on
             * whether this is the state at data_02183b08. */
      int lentSlot = data_027c027c->unk_48[data_027c0284] != &data_02183b08;

      if (frontCo == 0) {
        return;
      }
      for (i = 0; i < SKILL_SET_WORDS; i++) {
        PLAYER(player).skills[0][i] = frontSkills[i];
        if (PartnerBoost1(player)) {
          PLAYER(player).skills[0][i] |=
              data_022a72c8[player][lentSlot].words[i];
        }
      }
      return;
    }
  }

  if (backCo != 0) {
    for (i = 0; i < SKILL_SET_WORDS; i++) {
      PLAYER(player).skills[1][i] = backSkills[i];
      if (PartnerBoost1(player)) {
        PLAYER(player).skills[1][i] |= data_022a72c8[player][1].words[i];
      }
    }
  }

  if (frontCo == 0) {
    return;
  }
  for (i = 0; i < SKILL_SET_WORDS; i++) {
    PLAYER(player).skills[0][i] = frontSkills[i];
    if (PartnerBoost1(player)) {
      PLAYER(player).skills[0][i] |= data_022a72c8[player][0].words[i];
      if (CheckSkillsEquipped(PLAYER(player).skills[1], SKILL_RANK_TEAMWORK)) {
        PartnerBoost2(PLAYER(player).skills[0], SKILL_PARTNER_TEAMWORK);
      }
      if (CheckSkillsEquipped(PLAYER(player).skills[1], SKILL_RANK_SYNERGY)) {
        PartnerBoost2(PLAYER(player).skills[0], SKILL_PARTNER_SYNERGY);
      }
      if (CheckSkillsEquipped(PLAYER(player).skills[1], SKILL_RANK_BODYGUARD)) {
        PartnerBoost2(PLAYER(player).skills[0], SKILL_PARTNER_BODYGUARD);
      }
    }
  }
}
