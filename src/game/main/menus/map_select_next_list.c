#include "types.h"
#include "game/map_select.h"

void Sound_PlaySe(int id);
void func_0205f12c(int slideBack);

extern MenuStateFrom400 data_0229ef30;

/* Switches the current tab to the next map list, wrapping to the first, and
 * starts the slide between them (func_0205f12c). The first switch plays a
 * sound. Remembers the map selected before. */
void MapSelect_NextList(void)
{
    if (MENU->setup.listChanged == 0) {
        Sound_PlaySe(0x10002);
        MENU->setup.listChanged = 1;
    }
    data_0229ef30.prevMapId = SELECTED_MAP_ID();
    MENU->setup.prevList = LIST_OF_TAB(TAB());
    if (LIST_OF_TAB(TAB()) < MENU->setup.mapListCount - 1) {
        LIST_OF_TAB(TAB())++;
    } else {
        LIST_OF_TAB(TAB()) = 0;
    }
    func_0205f12c(1);
}
