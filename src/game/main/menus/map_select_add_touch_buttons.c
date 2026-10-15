#include "types.h"
#include "game/map_select.h"
#include "game/menu_button.h"

int Link_IsActive(void);
int Link_GetOwnPlayer(void);

/* Touch buttons of the map select screen, 32x32 each. */
extern const MenuButtonTemplate data_0215c8c8; /* (224, 160), func_0205e898 */
extern const MenuButtonTemplate
    data_0215c940; /* (224, 128), MapSelect_NextPage */
extern const MenuButtonTemplate data_0215c878; /* (224, 96), previous page */
extern const MenuButtonTemplate data_0215c918; /* (32, 0), previous list */
extern const MenuButtonTemplate data_0215c850; /* (192, 0), next list */
extern const MenuButtonTemplate
    data_0215c828; /* (32, 32), MapSelect_PrevSubStripKey */
extern const MenuButtonTemplate
    data_0215c8f0; /* (192, 32), MapSelect_NextSubStripKey */

/* Adds the touch buttons of the map select screen (ids 0xf8-0xfe), unless
 * this is link play and Link_GetOwnPlayer is set. With two maps outside the War
 * Room the list buttons move 16 pixels down; two more buttons appear only
 * in the War Room. */
void MapSelect_AddTouchButtons(void) {
  if (Link_IsActive() == 1 && Link_GetOwnPlayer() != 0) {
    return;
  }
  MenuButton_Add(0, 0, &data_0215c8c8, 0xfe);
  MenuButton_Add(0, 0, &data_0215c940, 0xfc);
  MenuButton_Add(0, 0, &data_0215c878, 0xfd);
  if (MAP_STATE->gameMode == GAME_MODE_WAR_ROOM || MAP_STATE->twoMaps == 0) {
    MenuButton_Add(0, 0, &data_0215c918, 0xfb);
    MenuButton_Add(0, 0, &data_0215c850, 0xfa);
  } else {
    MenuButton_Add(0, 16, &data_0215c918, 0xfb);
    MenuButton_Add(0, 16, &data_0215c850, 0xfa);
  }
  if (MAP_STATE->gameMode == GAME_MODE_WAR_ROOM) {
    MenuButton_Add(0, 0, &data_0215c828, 0xf9);
    MenuButton_Add(0, 0, &data_0215c8f0, 0xf8);
  }
}
