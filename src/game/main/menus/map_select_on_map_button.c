#include "types.h"
#include "game/menu_button.h"

u32 Link_IsActive(void);
void EventQueue_Push(int id, int arg);
void MapSelect_SelectPagePosition(int pos);

/* Button handler of a map on the page: the button id gives the map's position
 * on the page. In link mode (Link_IsActive returns 1) the choice is queued as
 * event 4 instead of applied. */
void MapSelect_OnMapButton(MenuButton* button) {
  if (Link_IsActive() == 1) {
    EventQueue_Push(4, button->id);
  } else {
    MapSelect_SelectPagePosition(button->id);
  }
}
