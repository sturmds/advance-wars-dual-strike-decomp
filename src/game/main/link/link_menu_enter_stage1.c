#include "types.h"
#include "game/Unk_0229fb30.h"
#include "game/menu_button.h"

void Sound_PlaySe(u32 soundId);
void HelpText_Show(u32 textId, int tile);
int func_0207a260(void);
void LinkMenu_ResumeMarker(int index);
void LinkMenu_AddStage1Buttons(void);

/* Back to stage 1 of the two-player link menu: stops the three marker tasks and
 * rebuilds the stage 1 buttons. Button 254 is closed when leaving stage 3 on
 * the side where func_0207a260() is not 0. */
void LinkMenu_EnterStage1(void) {
  Sound_PlaySe(0x10009);
  LinkMenu_ResumeMarker(0);
  LinkMenu_ResumeMarker(1);
  LinkMenu_ResumeMarker(2);
  MenuButtons_RemoveSelectable();
  if (func_0207a260() != 0 && data_0229fb30.unk_630 == 3) {
    MenuButton_Close(254);
  }
  data_0229fb30.unk_630 = 1;
  LinkMenu_AddStage1Buttons();
  HelpText_Show(0x48000004, 960);
}
