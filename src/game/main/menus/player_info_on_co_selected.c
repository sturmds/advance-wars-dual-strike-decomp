#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

void Sound_PlaySe(u32 soundId);
void PlayerInfo_LoadCoPictures(void);
void PlayerInfo_StartRedrawTask(void);

/* A CO button of the player info screen became the selected one: shows that
 * player and CO (ids as PlayerInfo_AddCoButtons gives them) and
 * redraws the screen. */
void PlayerInfo_OnCoSelected(MenuButton* button) {
  Sound_PlaySe(0x10002);
  gMenuState.unk_0206d67c.player = (button->id >> 1) + 1;
  gMenuState.unk_0206d67c.slot = button->id & 1;
  PlayerInfo_StartRedrawTask();
  PlayerInfo_LoadCoPictures();
}
