#include "types.h"
#include "game/screen_other_map.h"

/* Draws map tile (mapX, mapY) into the BG tile maps. */
void MapView_DrawUnitTile(int mapX, int mapY);

#define GAME (data_027c027c->unk_48[data_027c0284])
#define VIEW (GAME->unk_1c4)

/* Redraws the 17 by 13 map tiles of the view, then runs the screen
 * config's unk_08 hook. */
void MapView_DrawAllTiles(void)
{
    u16 col;
    u16 row;

    for (row = 0; row < 13; row++) {
        for (col = 0; col < 17; col++) {
            MapView_DrawUnitTile((s16)(col + (VIEW->scrollX >> 4)), (s16)(row + (VIEW->scrollY >> 4)));
        }
    }
    if (data_027c027c->unk_50[data_027c0284]->unk_08 != NULL) {
        data_027c027c->unk_50[data_027c0284]->unk_08();
    }
}
