#include "types.h"
#include "game/force_rank_screen.h"
#include "game/menu_button.h"

void Sound_PlaySe(u32 soundId);

/* onConfirm of a slot button (ids 0xf0 + slot): makes that slot the one
 * that the next picked skill goes to. */
void ForceRankScreen_SelectSlot(MenuButton* button) {
  int slot = button->id - 0xf0;

  if (slot == gForceRankScreen->selectedSlot) {
    return;
  }
  if (slot > gForceRankScreen->rank) {
    return;
  }
  Sound_PlaySe(0x10002);
  gForceRankScreen->selectedSlot = slot;
}
