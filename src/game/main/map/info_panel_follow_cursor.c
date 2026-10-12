#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Points the info panel at the tile under the cursor. */
void InfoPanel_FollowCursor(void) {
  STATE->infoTileX = STATE->cursorX;
  STATE->infoTileY = STATE->cursorY;
}
