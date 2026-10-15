#include "types.h"
#include "game/map_select.h"
#include "game/menu_button.h"

int Link_IsActive(void);
int Link_GetOwnPlayer(void);
int Link_FindSlotOfPlayer(int player);
int VsSetup_GetPanelTileX(int player);
u32 Link_GetConnectedSlotMask(void);
BOOL Skills_AreDisabled(void);

extern MenuButtonTemplate data_0215cb48;
extern MenuButtonTemplate data_0215cb70;
extern MenuButtonTemplate data_0215cd50;
extern MenuButtonTemplate data_0215cda0;
extern MenuButtonTemplate data_0215cdf0;
extern MenuButtonTemplate data_0215ce18;
extern MenuButtonTemplate data_0215ce40;

static inline BOOL IsFirstLinkPlayer(void) { return Link_GetOwnPlayer() == 0; }

/* Adds buttons 250-254 of the Vs setup screen (250-252 only when skills are
 * on) and, per player slot, the buttons above (y 8, data_0215cdf0) and
 * below (y 40, data_0215cb70) the map buttons at y 24, ids 16 + player * 4
 * to + 3, and above (y 64) and below (y 128) the CO buttons at y 80, ids
 * 32 + player * 4 to + 3. Up and down arrows is a guess. In a link game only
 * the own slot gets them, plus, for the first link player, the CO arrows of
 * each slot not set in the Link_GetConnectedSlotMask mask; only the first link player
 * gets button 254. */
void VsSetup_AddArrowButtons(void) {
  int player;

  if (Link_IsActive() == 1) {
    if (IsFirstLinkPlayer() == 1) {
      MenuButton_Add(0, 0, &data_0215ce18, 254);
    }
    MenuButton_Add(0, 0, &data_0215ce40, 253);
    if (!Skills_AreDisabled()) {
      MenuButton_Add(0, 0, &data_0215cd50, 252);
      MenuButton_Add(56, 152, &data_0215cda0, 251);
      MenuButton_Add(152, 152, &data_0215cb48, 250);
    }
    player = Link_FindSlotOfPlayer(Link_GetOwnPlayer()) - 1;
    if (MAP_STATE->twoMaps == 1) {
      MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 28, 8, &data_0215cdf0,
                     player * 4 + 18);
      MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 28, 40, &data_0215cb70,
                     player * 4 + 19);
    }
    MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 4, 64, &data_0215cdf0,
                   player * 4 + 32);
    MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 4, 128, &data_0215cb70,
                   player * 4 + 33);
    MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 28, 64, &data_0215cdf0,
                   player * 4 + 34);
    MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 28, 128, &data_0215cb70,
                   player * 4 + 35);
    if (IsFirstLinkPlayer() == 1) {
      for (player = 0; player < MENU->setup.unk_2e3; player++) {
        if ((Link_GetConnectedSlotMask() & (1 << player)) == 0) {
          MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 4, 64,
                         &data_0215cdf0, player * 4 + 32);
          MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 4, 128,
                         &data_0215cb70, player * 4 + 33);
          MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 28, 64,
                         &data_0215cdf0, player * 4 + 34);
          MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 28, 128,
                         &data_0215cb70, player * 4 + 35);
        }
      }
    }
  } else {
    MenuButton_Add(0, 0, &data_0215ce18, 254);
    MenuButton_Add(0, 0, &data_0215ce40, 253);
    if (!Skills_AreDisabled()) {
      MenuButton_Add(0, 0, &data_0215cd50, 252);
      MenuButton_Add(56, 152, &data_0215cda0, 251);
      MenuButton_Add(152, 152, &data_0215cb48, 250);
    }
    for (player = 0; player < MENU->setup.unk_2e3; player++) {
      if (MAP_STATE->twoMaps == 0) {
        MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 16, 8,
                       &data_0215cdf0, player * 4 + 16);
        MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 16, 40,
                       &data_0215cb70, player * 4 + 17);
      } else {
        MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 4, 8, &data_0215cdf0,
                       player * 4 + 16);
        MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 4, 40,
                       &data_0215cb70, player * 4 + 17);
        MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 28, 8,
                       &data_0215cdf0, player * 4 + 18);
        MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 28, 40,
                       &data_0215cb70, player * 4 + 19);
      }
      MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 4, 64, &data_0215cdf0,
                     player * 4 + 32);
      MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 4, 128, &data_0215cb70,
                     player * 4 + 33);
      MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 28, 64, &data_0215cdf0,
                     player * 4 + 34);
      MenuButton_Add(VsSetup_GetPanelTileX(player) * 8 + 28, 128,
                     &data_0215cb70, player * 4 + 35);
    }
  }
}
