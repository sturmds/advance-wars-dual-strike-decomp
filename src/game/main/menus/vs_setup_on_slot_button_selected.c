#include "types.h"
#include "game/menu_state.h"

int Sound_PlaySe(int);

/* Plays the cursor sound and stores the slot (button id 48-51 minus 48) in
 * setup.selectedSlot. Called when the cursor moves to a slot button, a guess. */
void VsSetup_OnSlotButtonSelected(u8* buttonId) {
  Sound_PlaySe(0x10002);
  gMenuState.setup.selectedSlot = *buttonId - 48;
}
