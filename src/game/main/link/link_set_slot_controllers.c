#include "types.h"
#include "game/screen_other_map.h"

int Link_IsActive(void);
u32 Link_GetConnectedSlotMask(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* In link mode, sets the controller of slots 1-4: human for a slot whose bit
 * is set in the Link_GetConnectedSlotMask mask, else AI. The other screen's map state
 * (data_027c0288.unk_04) gets AI for every slot. */
void Link_SetSlotControllers(void) {
  int slot;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (Link_IsActive() == 0) {
    return;
  }
  for (slot = 0; slot < 4; slot++) {
    if ((1 << slot) & Link_GetConnectedSlotMask()) {
      STATE->slotControllers[slot + 1] = PLAYER_CONTROLLER_HUMAN;
    } else {
      STATE->slotControllers[slot + 1] = PLAYER_CONTROLLER_AI;
    }
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    STATE->slotControllers[slot + 1] = PLAYER_CONTROLLER_AI;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
}
