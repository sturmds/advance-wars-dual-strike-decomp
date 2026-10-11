#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Puts the side panel and the info panel at rest on the right of the screen
 * when cursorX is below 8, on the left
 * otherwise. Then points the info panel at the tile (cursorX, cursorY). */
void Panels_PlaceForCursor(void)
{
    if (STATE->cursorX < 8) {
        STATE->sidePanelRight = 1;
        STATE->sidePanelX = 192;
        STATE->infoPanelX = 164;
    } else {
        STATE->infoPanelRight = 0;
        STATE->sidePanelX = 0;
        STATE->infoPanelX = 3;
    }
    STATE->infoTileX = STATE->cursorX;
    STATE->infoTileY = STATE->cursorY;
}
