#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

extern MenuState gMenuState;
extern MenuButtonTemplate data_0215d358;

void Sound_PlaySe(u32 soundId);
void PlayerInfo_StartRedrawTask(void);

/* Turns the player info screen to the previous page. The previous page button
 * (253) goes away on page 0; leaving the last page brings back the next page
 * button (252) and removes 251. Needed to match: the first read of the page is
 * volatile, because the original loads it again for the decrement. */
void PlayerInfo_PrevPage(MenuButton* button) {
  if (*(volatile u8*)&gMenuState.unk_0206d67c.page == 0) {
    return;
  }
  gMenuState.unk_0206d67c.page--;
  Sound_PlaySe(0x10002);
  if (gMenuState.unk_0206d67c.page == 0) {
    MenuButton_Close(253);
  }
  if (gMenuState.unk_0206d67c.page == 2) {
    MenuButton_Add(0, 0, &data_0215d358, 252);
  }
  if (gMenuState.unk_0206d67c.page == 2) {
    MenuButton_Close(251);
  }
  PlayerInfo_StartRedrawTask();
}
