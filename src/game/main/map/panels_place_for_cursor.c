#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Puts the side panel and the info panel at rest on the right of the screen
 * when unk_28 is below 8 (the tile x of the AiShowAction steps), on the left
 * otherwise. Then points the info panel at the tile (unk_28, unk_2a). */
void Panels_PlaceForCursor(void)
{
    if (STATE->unk_28 < 8) {
        STATE->sidePanelRight = 1;
        STATE->sidePanelX = 192;
        STATE->infoPanelX = 164;
    } else {
        STATE->unk_c4 = 0;
        STATE->sidePanelX = 0;
        STATE->infoPanelX = 3;
    }
    STATE->infoTileX = STATE->unk_28;
    STATE->infoTileY = STATE->unk_2a;
}
