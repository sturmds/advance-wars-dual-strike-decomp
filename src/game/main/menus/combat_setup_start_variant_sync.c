#include "types.h"
#include "game/Unk_02160ac0.h"
#include "game/task.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

void LinkCoVariantSyncTask_Create(Task* parent, int slot0, int slot1, int slot2,
                                  u8 slot3);

/* Script step of the Combat setup task: starts the CO variant exchange as a child, giving it
 * the player of each of the four rows (0xff for a row without a player). */
void CombatSetup_StartVariantSync(Task* task) {
  u8 players[4];
  int i;

  for (i = 0; i < COMBAT_ROW_COUNT; i++) {
    if (gMenuState.combat.rows[i].memberCount == 0) {
      players[i] = 0xff;
    } else {
      players[i] = gMenuState.combat.rows[i].player;
    }
  }
  LinkCoVariantSyncTask_Create(task, players[0], players[1], players[2],
                               players[3]);
}
