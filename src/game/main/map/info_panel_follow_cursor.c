#include "types.h"
#include "game/Unk_02080440.h"

/* Points the info panel at the tile under the cursor. */
void InfoPanel_FollowCursor(void) {
  MAP_STATE->infoTileX = MAP_STATE->cursorX;
  MAP_STATE->infoTileY = MAP_STATE->cursorY;
}
