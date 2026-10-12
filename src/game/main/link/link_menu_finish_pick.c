#include "types.h"
#include "game/Unk_0229fb30.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

void Sound_PlaySe(u32 soundId);
int func_0207a260(void);
void LinkMenu_ResumeMarker(u8 index);
void LinkMenu_RestoreStage2Buttons(void);

/* Called for a player whose pick marker is up: clears the flag and ends the marker task. On the
 * local player's side it goes on to stage 2. */
void LinkMenu_FinishPick(int player) {
  u8* picked = (u8*)&gMenuState + player;

  if (!picked[LINK_PICKED_OFFSET]) {
    return;
  }
  picked[LINK_PICKED_OFFSET] = 0;
  Sound_PlaySe(0x10009);
  LinkMenu_ResumeMarker(player + 1);
  if (player != func_0207a260()) {
    return;
  }
  data_0229fb30.unk_630 = 2;
  LinkMenu_RestoreStage2Buttons();
}
