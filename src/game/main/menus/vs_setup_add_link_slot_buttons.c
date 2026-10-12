#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

int Link_GetOwnPlayer(void);
int VsSetup_GetSlotTileX(int player);
u32 Link_GetConnectedSlotMask(void);

extern MenuState gMenuState;
extern MenuButtonTemplate data_0215cbe8;
extern MenuButtonTemplate data_0215cc10;
extern MenuButtonTemplate data_0215cc38;
extern MenuButtonTemplate data_0215cc60;

static inline BOOL IsFirstLinkPlayer(void) { return Link_GetOwnPlayer() == 0; }

/* Adds the buttons of a link player's slot on the Vs setup screen again
 * (VsSetup_ClearSlotReady calls it when the player leaves the ready state): button
 * 253, the slot button 48 + player at y 64 and the buttons above (y 32) and
 * below (y 96) it, ids 52 + player * 2 and + 1. The first link player also
 * gets them for each slot not set in the Link_GetConnectedSlotMask mask; the others
 * close button 254. */
void VsSetup_AddLinkSlotButtons(int player) {
  int other;

  MenuButton_Add(0, 0, &data_0215cc10, 253);
  MenuButton_Add(VsSetup_GetSlotTileX(player) * 8 + 16, 32, &data_0215cc60,
                 player * 2 + 52);
  MenuButton_Add(VsSetup_GetSlotTileX(player) * 8 + 16, 96, &data_0215cbe8,
                 player * 2 + 53);
  MenuButton_Add(VsSetup_GetSlotTileX(player) * 8 + 8, 64, &data_0215cc38,
                 player + 48);
  if (IsFirstLinkPlayer() == 1) {
    for (other = 0; other < gMenuState.setup.unk_2e3; other++) {
      if ((Link_GetConnectedSlotMask() & (1 << other)) == 0) {
        MenuButton_Add(VsSetup_GetSlotTileX(other) * 8 + 8, 64, &data_0215cc38,
                       other + 48);
        MenuButton_Add(VsSetup_GetSlotTileX(other) * 8 + 16, 32, &data_0215cc60,
                       other * 2 + 52);
        MenuButton_Add(VsSetup_GetSlotTileX(other) * 8 + 16, 96, &data_0215cbe8,
                       other * 2 + 53);
      }
    }
  }
  if (IsFirstLinkPlayer() == 0) {
    MenuButton_Close(254);
  }
}
