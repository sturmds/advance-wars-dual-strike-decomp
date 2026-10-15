#include "types.h"
#include "game/map_select.h"

void Sound_PlaySe(int id);
void MapSelect_StartListSlide(int slideBack);

/* Like MapSelect_NextList, but moves to the previous list, wrapping to the last. */
void MapSelect_PrevList(void) {
  if (MENU->setup.listChanged == 0) {
    Sound_PlaySe(0x10002);
    MENU->setup.listChanged = 1;
  }
  gMenuState.setup.prevMapId = SELECTED_MAP_ID();
  MENU->setup.prevList = LIST_OF_TAB(TAB());
  if (LIST_OF_TAB(TAB()) != 0) {
    LIST_OF_TAB(TAB())--;
  } else {
    LIST_OF_TAB(TAB()) = MENU->setup.mapListCount - 1;
  }
  MapSelect_StartListSlide(0);
}
