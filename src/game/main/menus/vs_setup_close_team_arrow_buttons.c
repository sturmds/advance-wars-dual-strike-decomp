#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

int Link_GetOwnPlayer(void);
u32 Link_GetConnectedSlotMask(void);

extern MenuState gMenuState;
extern MenuButtonTemplate data_0215ccd8;

static inline BOOL IsFirstLinkPlayer(void) { return Link_GetOwnPlayer() == 0; }

/* After a slot is ready: closes its team arrow buttons (52 + slot * 2 and
 * 53 + slot * 2) and button 253. The first link player also closes the
 * arrows of slots not set in the Link_GetConnectedSlotMask mask; any other player gets
 * button 254. */
void VsSetup_CloseTeamArrowButtons(int slot) {
  int player;

  MenuButtons_RemoveSelectable();
  MenuButton_Close(slot * 2 + 52);
  MenuButton_Close(slot * 2 + 53);
  MenuButton_Close(253);
  if (IsFirstLinkPlayer() == 1) {
    for (player = 0; player < gMenuState.setup.unk_2e3; player++) {
      if ((Link_GetConnectedSlotMask() & (1 << player)) == 0) {
        MenuButton_Close(player * 2 + 52);
        MenuButton_Close(player * 2 + 53);
      }
    }
  }
  if (IsFirstLinkPlayer() != 0) {
    return;
  }
  MenuButton_Add(0, 0, &data_0215ccd8, 254);
}
