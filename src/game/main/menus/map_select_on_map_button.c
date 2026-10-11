#include "types.h"
#include "game/menu_button.h"

u32 func_0207bd80(void);
void EventQueue_Push(int id, int arg);
void MapSelect_SelectPagePosition(int pos);

/* Button handler of a map on the page: the button id gives the map's position
 * on the page. In link mode (func_0207bd80 returns 1) the choice is queued as
 * event 4 instead of applied. */
void MapSelect_OnMapButton(MenuButton *button)
{
    if (func_0207bd80() == 1) {
        EventQueue_Push(4, button->id);
    } else {
        MapSelect_SelectPagePosition(button->id);
    }
}
