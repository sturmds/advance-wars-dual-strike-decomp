#include "types.h"
#include "game/Unk_02080440.h"
/* Needed to match: data_022a7a9c is read with its rows (as in
 * battle_results_fill.c), so hide the header's one-entry declaration. */
#define data_022a7a9c data_022a7a9c_entry
#include "game/Unk_022a7a9c.h"
#undef data_022a7a9c

extern BattleResults data_022a7a9c;

int Co_GetExperience(int co);
void Co_SetExperience(int co, int experience);
void PlayRecords_UpdateCoRankTotal(void);

/* Adds the experience gain in each result row to the COs of human players,
 * in every mode but Survival (it has its own, Survival_AwardCoExperience). */
void BattleResults_AwardExperience(void) {
  int gain;
  int co;
  int player;
  int slot;

  if (MAP_STATE->gameMode == GAME_MODE_SURVIVAL) {
    return;
  }
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (MAP_STATE->players[player].controller == PLAYER_CONTROLLER_HUMAN) {
      for (slot = 0; slot < PLAYER_CO_COUNT; slot++) {
        co = (&data_022a7a9c.rows[player].unk_00)[slot] & PLAYER_CO_ID_MASK;
        if (co != 0) {
          gain = data_022a7a9c.rows[player].expGain[slot];
          gain += Co_GetExperience(co);
          Co_SetExperience(co, gain);
        }
      }
    }
  }
  PlayRecords_UpdateCoRankTotal();
}
