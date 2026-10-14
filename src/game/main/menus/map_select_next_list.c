#include "types.h"
#include "game/map_select.h"

extern MenuState gMenuState;

void Sound_PlaySe(int id);
void MapSelect_StartListSlide(int slideBack);

/* Switches the current tab to the next map list, wrapping to the first, and
 * starts the slide between them (MapSelect_StartListSlide). The first switch plays a
 * sound. Remembers the map selected before. */
void MapSelect_NextList(void) {
  if (MENU->setup.listChanged == 0) {
    Sound_PlaySe(0x10002);
    MENU->setup.listChanged = 1;
  }
  gMenuState.setup.prevMapId = SELECTED_MAP_ID();
  MENU->setup.prevList = LIST_OF_TAB(TAB());
  if (LIST_OF_TAB(TAB()) < MENU->setup.mapListCount - 1) {
    LIST_OF_TAB(TAB())++;
  } else {
    LIST_OF_TAB(TAB()) = 0;
  }
  MapSelect_StartListSlide(1);
}
