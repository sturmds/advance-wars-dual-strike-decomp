#include "types.h"
#include "game/Unk_0229fb30.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

void Sound_PlaySe(u32 soundId);
int func_0207a260(void);
void LinkMenu_CreateMarker(u8 kind, u16 x, u16 y);
void LinkMenu_AddBackButton(void);

/* A player picks a stage 2 button: puts that player's marker next to it and remembers the pick.
 * On the local player's side it goes on to stage 3. */
void LinkMenu_PickButton(int player, int button) {
  u8* picked = (u8*)&gMenuState + player;

  if (picked[LINK_PICKED_OFFSET]) {
    return;
  }
  Sound_PlaySe(0x10006);
  LinkMenu_CreateMarker(player + 1, player * 96 + 64, button * 16 + 120);
  picked[LINK_PICKED_OFFSET] = 1;
  if (player == data_0229fb30.unk_631) {
    data_0229fb30.unk_632 = button;
  } else {
    data_0229fb30.unk_633 = button;
  }
  if (player != func_0207a260()) {
    return;
  }
  MenuButtons_RemoveSelectable();
  data_0229fb30.unk_630 = 3;
  LinkMenu_AddBackButton();
}
