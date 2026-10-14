#include "types.h"
#include "game/player.h"
#include "game/screen_other_map.h"

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Step of the Versus task script data_021673dc (after VersusTask_ResetGame):
 * slot 1 is played by a human and slots 2-4 by the AI; the other screen's
 * map state gets the AI in every slot. Then income is 1000 per property
 * and unk_80 and unk_81 are cleared. */
void VersusTask_SetDefaultRules(void) {
  int slot;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  for (slot = 1; slot <= PLAYER_SLOT_LAST; slot++) {
    if (slot == 1) {
      STATE->slotControllers[slot] = PLAYER_CONTROLLER_HUMAN;
    } else {
      STATE->slotControllers[slot] = PLAYER_CONTROLLER_AI;
    }
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    STATE->slotControllers[slot] = PLAYER_CONTROLLER_AI;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  STATE->fundsPerProperty = 1000;
  STATE->dayLimit = 0;
  STATE->propertyGoal = 0;
}
