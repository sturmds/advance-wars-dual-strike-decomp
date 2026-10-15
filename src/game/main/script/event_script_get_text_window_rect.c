#include "types.h"
#include "game/rect.h"
#include "sdk/os.h"

extern s16 gEventTextWindowPos;

/* Fills rect with the area of the event text window for the screen mode in
 * gEventTextWindowPos: 0 is the top window, 1 the one at y 144. Any other mode
 * terminates. */
void EventScript_GetTextWindowRect(Rect* rect) {
  switch (gEventTextWindowPos) {
    case 0:
      rect->x = 0;
      rect->y = 0;
      rect->width = 256;
      rect->height = 48;
      break;
    case 1:
      rect->x = 0;
      rect->y = 144;
      rect->width = 256;
      rect->height = 192;
      break;
    default:
      OS_Terminate();
      break;
  }
}
