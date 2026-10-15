#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

int Link_IsActive(void);
int Link_GetOwnPlayer(void);
int Link_FindSlotOfPlayer(int player);
int VsSetup_GetSlotTileX(int player);
u32 Link_GetConnectedSlotMask(void);

extern MenuButtonTemplate data_0215cbe8;
extern MenuButtonTemplate data_0215cc10;
extern MenuButtonTemplate data_0215cc60;
extern MenuButtonTemplate data_0215ccd8;

static inline BOOL IsFirstLinkPlayer(void) { return Link_GetOwnPlayer() == 0; }

/* Adds buttons 253 and 254 of the team screen and, per player slot, the
 * arrows above (y 32, id 52 + player * 2) and below (y 96, id 53 + player
 * * 2) the slot button, which step the slot's team. In a link
 * game only the own slot gets arrows, plus, for the first link player,
 * each slot not set in the Link_GetConnectedSlotMask mask; only the first link player
 * gets button 254. */
void VsSetup_AddTeamArrowButtons(void) {
  int player;

  if (Link_IsActive() == 1) {
    if (IsFirstLinkPlayer() == 1) {
      MenuButton_Add(0, 0, &data_0215ccd8, 254);
    }
    MenuButton_Add(0, 0, &data_0215cc10, 253);
    player = Link_FindSlotOfPlayer(Link_GetOwnPlayer()) - 1;
    MenuButton_Add(VsSetup_GetSlotTileX(player) * 8 + 16, 32, &data_0215cc60,
                   player * 2 + 52);
    MenuButton_Add(VsSetup_GetSlotTileX(player) * 8 + 16, 96, &data_0215cbe8,
                   player * 2 + 53);
    if (IsFirstLinkPlayer() != 1) {
      return;
    }
    for (player = 0; player < gMenuState.setup.unk_2e3; player++) {
      if ((Link_GetConnectedSlotMask() & (1 << player)) == 0) {
        MenuButton_Add(VsSetup_GetSlotTileX(player) * 8 + 16, 32,
                       &data_0215cc60, player * 2 + 52);
        MenuButton_Add(VsSetup_GetSlotTileX(player) * 8 + 16, 96,
                       &data_0215cbe8, player * 2 + 53);
      }
    }
  } else {
    MenuButton_Add(0, 0, &data_0215ccd8, 254);
    MenuButton_Add(0, 0, &data_0215cc10, 253);
    for (player = 0; player < gMenuState.setup.unk_2e3; player++) {
      MenuButton_Add(VsSetup_GetSlotTileX(player) * 8 + 16, 32, &data_0215cc60,
                     player * 2 + 52);
      MenuButton_Add(VsSetup_GetSlotTileX(player) * 8 + 16, 96, &data_0215cbe8,
                     player * 2 + 53);
    }
  }
}
