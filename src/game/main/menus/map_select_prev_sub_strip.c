#include "types.h"
#include "game/map_select.h"

void Sound_PlaySe(int id);
void MapSelect_StartSubSlide(int slideBack);

/* Like MapSelect_NextSubStrip, but moves back and wraps to the last strip. */
void MapSelect_PrevSubStrip(void)
{
    if (MENU->setup.listChanged == 0) {
        Sound_PlaySe(0x10002);
        MENU->setup.listChanged = 1;
    }
    if (TABLES.subStrip != 0) {
        TABLES.subStrip--;
    } else {
        TABLES.subStrip = 3;
    }
    MapSelect_StartSubSlide(0);
}
