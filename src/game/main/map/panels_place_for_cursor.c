#include "types.h"
#include "game/Unk_02080440.h"

/* Puts the side panel and the info panel at rest on the right of the screen
 * when cursorX is below 8, on the left
 * otherwise. Then points the info panel at the tile (cursorX, cursorY). */
void Panels_PlaceForCursor(void) {
  if (MAP_STATE->cursorX < 8) {
    MAP_STATE->sidePanelRight = 1;
    MAP_STATE->sidePanelX = 192;
    MAP_STATE->infoPanelX = 164;
  } else {
    MAP_STATE->infoPanelRight = 0;
    MAP_STATE->sidePanelX = 0;
    MAP_STATE->infoPanelX = 3;
  }
  MAP_STATE->infoTileX = MAP_STATE->cursorX;
  MAP_STATE->infoTileY = MAP_STATE->cursorY;
}
