#include "types.h"
#include "game/event_script.h"

void Bg_FillRect(u16* map, int x, int y, int width, int height, int tile);

/* Clears the 32 x 6 tile map buffer gEventTextMap (192 entries). */
void EventScript_ClearTextMap(void) {
  Bg_FillRect(gEventTextMap[0], 0, 0, 32, 6, 0);
}
