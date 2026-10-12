#include "types.h"
#include "game/menu_button.h"
#include "game/link_menu_state.h"

void Sound_PlaySe(u32 soundId);

#define SE_MOVE 0x10002

/* onSelect of the row buttons: plays the cursor sound and remembers the button. */
void LinkMenu_OnButtonSelect(MenuButton* button) {
  Sound_PlaySe(SE_MOVE);
  data_0215fed0->selected = button->id;
}
