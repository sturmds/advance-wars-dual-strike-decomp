#include "types.h"
#include "game/map_select.h"

void Sound_PlaySe(int id);
void MapSelect_StartSubSlide(int slideBack);

/* Moves to the next of the four strips (TABLES.subStrip), wrapping to the
 * first, and starts the slide. The first change of a list plays a sound. */
void MapSelect_NextSubStrip(void) {
  if (MENU->setup.listChanged == 0) {
    Sound_PlaySe(0x10002);
    MENU->setup.listChanged = 1;
  }
  if (TABLES.subStrip < 3) {
    TABLES.subStrip++;
  } else {
    TABLES.subStrip = 0;
  }
  MapSelect_StartSubSlide(1);
}
