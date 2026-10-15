#include "types.h"
#include "game/map_select.h"
#include "game/menu_button.h"

int Link_IsActive(void);
int Link_GetOwnPlayer(void);
int Link_FindSlotOfPlayer(int player);
int VsSetup_GetPanelTileX(int player);
u32 Link_GetConnectedSlotMask(void);

extern MenuButtonTemplate data_0215cb20;
extern MenuButtonTemplate data_0215cb98;

static inline BOOL IsFirstLinkPlayer(void) { return Link_GetOwnPlayer() == 0; }

/* Buttons of the Vs setup screen: per player, a map button row at y 24 (ids
 * player * 2 and + 1, two with twoMaps) and two CO buttons at y 80 (ids
 * 8 + player * 2 and + 1). Link games add the own slot, and the other
 * slots for player 0. Needed to match: one variable holds the own slot and
 * both loop counters. */
void VsSetup_AddMapAndCoButtons(void) {
  int player;

  if (Link_IsActive() == 1) {
    player = Link_FindSlotOfPlayer(Link_GetOwnPlayer()) - 1;
    if (MAP_STATE->twoMaps == 1) {
      MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 28, 24, &data_0215cb20,
                     player * 2 + 1);
    }
    MenuButton_Add(VsSetup_GetPanelTileX(player) * 8, 80, &data_0215cb98,
                   player * 2 + 8);
    MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 24, 80, &data_0215cb98,
                   player * 2 + 9);
    if (IsFirstLinkPlayer() == 1) {
      for (player = 0; player < MENU->setup.unk_2e3; player++) {
        if ((Link_GetConnectedSlotMask() & (1 << player)) == 0) {
          MenuButton_Add(VsSetup_GetPanelTileX(player) * 8, 80, &data_0215cb98,
                         player * 2 + 8);
          MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 24, 80,
                         &data_0215cb98, player * 2 + 9);
        }
      }
    }
  } else {
    for (player = 0; player < MENU->setup.unk_2e3; player++) {
      if (MAP_STATE->twoMaps == 0) {
        MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 16, 24,
                       &data_0215cb20, player * 2);
      } else {
        MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 4, 24,
                       &data_0215cb20, player * 2);
        MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 28, 24,
                       &data_0215cb20, player * 2 + 1);
      }
      MenuButton_Add(VsSetup_GetPanelTileX(player) * 8, 80, &data_0215cb98,
                     player * 2 + 8);
      MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 24, 80, &data_0215cb98,
                     player * 2 + 9);
    }
  }
}
